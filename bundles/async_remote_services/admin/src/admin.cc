/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <admin.h>
#include <celix_api.h>
#include <pubsub_message_serialization_service.h>

#define L_DEBUG(...) \
    celix_logHelper_log(_logger, CELIX_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define L_INFO(...) \
    celix_logHelper_log(_logger, CELIX_LOG_LEVEL_INFO, __VA_ARGS__)
#define L_WARN(...) \
    celix_logHelper_log(_logger, CELIX_LOG_LEVEL_WARNING, __VA_ARGS__)
#define L_ERROR(...) \
    celix_logHelper_log(_logger, CELIX_LOG_LEVEL_ERROR, __VA_ARGS__)

celix::async_rsa::AsyncAdmin::AsyncAdmin(std::shared_ptr<celix::dm::DependencyManager> &mng) noexcept : _mng(mng), _logger(celix_logHelper_create(mng->bundleContext(), "celix_async_rsa_admin")) {

}

celix::async_rsa::AsyncAdmin::~AsyncAdmin() {
    celix_logHelper_destroy(_logger);
}

void celix::async_rsa::AsyncAdmin::addEndpoint(celix::rsa::Endpoint* endpoint, [[maybe_unused]] Properties &&properties) {
    std::unique_lock l(_m);
    addEndpointInternal(*endpoint);
}

void celix::async_rsa::AsyncAdmin::removeEndpoint([[maybe_unused]] celix::rsa::Endpoint* endpoint, [[maybe_unused]] Properties &&properties) {
    auto interface = properties.get("service.exported.interfaces");

    if(interface.empty()) {
        L_DEBUG("Removing endpoint but no exported interfaces");
        return;
    }

    std::unique_lock l(_m);

    _toBeCreatedImportedEndpoints.erase(std::remove_if(_toBeCreatedImportedEndpoints.begin(), _toBeCreatedImportedEndpoints.end(), [&interface](auto const &endpoint){
        auto endpointInterface = endpoint.getProperties().get("service.exported.interfaces");
        return !endpointInterface.empty() && endpointInterface == interface;
    }), _toBeCreatedImportedEndpoints.end());

    auto svcId = properties.get(OSGI_FRAMEWORK_SERVICE_ID);

    if(svcId.empty()) {
        L_DEBUG("Removing endpoint but no service instance");
        return;
    }

    auto instanceIt = _serviceInstances.find(std::stol(svcId));

    if(instanceIt == end(_serviceInstances)) {
        return;
    }

    _mng->destroyComponent(instanceIt->second);
}

void celix::async_rsa::AsyncAdmin::addImportedServiceFactory(celix::async_rsa::IImportedServiceFactory *factory, Properties &&properties) {
    auto interface = properties.get("service.exported.interfaces");

    if(interface.empty()) {
        L_DEBUG("Adding service factory but no exported interfaces");
        return;
    }

    std::unique_lock l(_m);

    auto existingFactory = _factories.find(interface);

    if(existingFactory != end(_factories)) {
        L_WARN("Adding service factory but factory already exists");
        return;
    }

    _factories.emplace(interface, factory);

    for(auto it = _toBeCreatedImportedEndpoints.begin(); it != _toBeCreatedImportedEndpoints.end();) {
        auto tbceInterface = it->getProperties().get("service.exported.interfaces");
        if(tbceInterface.empty() || tbceInterface != interface) {
            it++;
        } else {
            addEndpointInternal(*it);
            _toBeCreatedImportedEndpoints.erase(it);
        }
    }
}

void celix::async_rsa::AsyncAdmin::removeImportedServiceFactory([[maybe_unused]] celix::async_rsa::IImportedServiceFactory *factory, Properties &&properties) {
    std::unique_lock l(_m);
    auto interface = properties.get("service.exported.interfaces");

    if(interface.empty()) {
        L_WARN("Removing service factory but missing exported interfaces");
        return;
    }

    _factories.erase(interface);
}

void celix::async_rsa::AsyncAdmin::addEndpointInternal(celix::rsa::Endpoint& endpoint) {

    const auto& properties = endpoint.getProperties();
    auto interface = properties.get("service.exported.interfaces");

    if(interface.empty()) {
        L_WARN("Adding endpoint but missing exported interfaces");
        return;
    }

    auto svcId = properties.get(OSGI_FRAMEWORK_SERVICE_ID);

    if(svcId.empty()) {
        L_WARN("Adding endpoint but missing service id");
        return;
    }

    auto existingFactory = _factories.find(interface);

    if(existingFactory == end(_factories)) {
        L_DEBUG("Adding endpoint but no factory available yet, delaying creation");
        _toBeCreatedImportedEndpoints.emplace_back(celix::rsa::Endpoint{properties});
        return;
    }

    L_DEBUG("Adding endpoint, created service");
    _serviceInstances.emplace(std::stol(svcId), existingFactory->second->create(_mng, (Properties&&) properties));
}

class AdminActivator {
public:
    explicit AdminActivator(std::shared_ptr<celix::dm::DependencyManager> mng) :
            _cmp(mng->createComponent(std::make_unique<celix::async_rsa::AsyncAdmin>(mng))), _mng(std::move(mng)) {

        _cmp.createServiceDependency<celix::rsa::Endpoint>()
                .setRequired(false)
                .setCallbacks(&celix::async_rsa::AsyncAdmin::addEndpoint, &celix::async_rsa::AsyncAdmin::removeEndpoint)
                .build();

        _cmp.createServiceDependency<celix::async_rsa::IImportedServiceFactory>()
                .setRequired(false)
                .setCallbacks(&celix::async_rsa::AsyncAdmin::addImportedServiceFactory, &celix::async_rsa::AsyncAdmin::removeImportedServiceFactory)
                .build();

        _cmp.build();
    }

    ~AdminActivator() noexcept {
        _mng->destroyComponent(_cmp);
    }

    AdminActivator(const AdminActivator &) = delete;
    AdminActivator &operator=(const AdminActivator &) = delete;
private:
    celix::dm::Component<celix::async_rsa::AsyncAdmin>& _cmp;
    std::shared_ptr<celix::dm::DependencyManager> _mng;
};

CELIX_GEN_CXX_BUNDLE_ACTIVATOR(AdminActivator)
