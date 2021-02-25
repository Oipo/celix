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

#ifndef CELIX_CONSTANTS_H_
#define CELIX_CONSTANTS_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Service property (named "objectClass") identifying the service name under which a service was registered
* in the Celix framework.
*
* This property is set by the Celix framework when a service is registered.
*/
#define OSGI_FRAMEWORK_OBJECTCLASS "objectClass"

/**
 * @brief Service property (named "service.id") identifying a service's registration number (of type long).
 *
 * The value of this property is assigned by the Celix framework when a service is registered.
 * The Celix framework assigns a unique value that is larger than all previously assigned values since the
 * Celix framework was started.
 */
#define OSGI_FRAMEWORK_SERVICE_ID "service.id"

#define OSGI_FRAMEWORK_SERVICE_PID "service.pid"

/**
 * @brief The bundle id (value 0) used to identify the Celix framework.
 */
#define CELIX_FRAMEWORK_BUNDLE_ID 0L

/**
 * @brief Service property (named "service.ranking") identifying a service's ranking number (of type long).
 *
 * This property may be supplied in the properties passed to the BundleContext::registerService method.
 * The service ranking is used by the Framework to order services for the service trackers.
 * Services with the highest ranking are first in the tracked services set and highest ranking service is used
 * when setting a service (tracking a single service).
 *
 * If services have the same service ranking, the oldest service precedes the older services (so lower service id
 * before higher service id).
 *
 * The default ranking is 0. A service with a ranking of LONG_MAX is very likely to be returned as the default
 * service, whereas a service with a ranking of LONG_MIN is very unlikely to be returned.
 */
#define OSGI_FRAMEWORK_SERVICE_RANKING "service.ranking"

/**
 * @brief Service property (named "service.version") specifying the optional version of a service.
 *
 * A version can be specified with major, minor and micro element in the form of <major>.<minor>.<micro>.
 * Examples:
 *   - 1.0.0
 *   - 2.1.2
 *
 *  Version can be filter with a filter range when using and/or tracking services. A filter range is specified
 *  using Maven-style version range:
 *   - [1,2) (version 1.0.0 (inclusive) until, but not including version 2.0.0 (exclusive)
 *   - [1.1.3, 1.5] (version 1.1.3 (inclusive) until version 1.5.0 (inclusive))
 */
#define CELIX_FRAMEWORK_SERVICE_VERSION "service.version"

/**
 * The service language property and values are deprecated
 */
#define CELIX_FRAMEWORK_SERVICE_LANGUAGE "service.lang"
#define CELIX_FRAMEWORK_SERVICE_C_LANGUAGE "C"
#define CELIX_FRAMEWORK_SERVICE_CXX_LANGUAGE "C++"
#define CELIX_FRAMEWORK_SERVICE_SHARED_LANGUAGE "shared" //e.g. marker services

#define OSGI_FRAMEWORK_BUNDLE_ACTIVATOR "Bundle-Activator"

#define OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_CREATE "celix_bundleActivator_create"
#define OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_START "celix_bundleActivator_start"
#define OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_STOP "celix_bundleActivator_stop"
#define OSGI_FRAMEWORK_BUNDLE_ACTIVATOR_DESTROY "celix_bundleActivator_destroy"

#define OSGI_FRAMEWORK_DEPRECATED_BUNDLE_ACTIVATOR_CREATE "bundleActivator_create"
#define OSGI_FRAMEWORK_DEPRECATED_BUNDLE_ACTIVATOR_START "bundleActivator_start"
#define OSGI_FRAMEWORK_DEPRECATED_BUNDLE_ACTIVATOR_STOP "bundleActivator_stop"
#define OSGI_FRAMEWORK_DEPRECATED_BUNDLE_ACTIVATOR_DESTROY "bundleActivator_destroy"

#define OSGI_FRAMEWORK_BUNDLE_SYMBOLICNAME "Bundle-SymbolicName"
#define OSGI_FRAMEWORK_BUNDLE_VERSION "Bundle-Version"
#define OSGI_FRAMEWORK_PRIVATE_LIBRARY "Private-Library"
#define OSGI_FRAMEWORK_EXPORT_LIBRARY "Export-Library"
#define OSGI_FRAMEWORK_IMPORT_LIBRARY "Import-Library"

/**
 * @brief Celix framework environment property (named "org.osgi.framework.storage") specifying the cache
 * directory used for the bundle caches.
 *
 * If not specified ".cache" is used.
 */
#define OSGI_FRAMEWORK_FRAMEWORK_STORAGE "org.osgi.framework.storage"

#define OSGI_FRAMEWORK_STORAGE_USE_TMP_DIR "org.osgi.framework.storage.use.tmp.dir"
#define OSGI_FRAMEWORK_FRAMEWORK_STORAGE_CLEAN_NAME "org.osgi.framework.storage.clean"
#define OSGI_FRAMEWORK_FRAMEWORK_STORAGE_CLEAN_DEFAULT true

/**
 * @brief Celix framework environment property (named "org.osgi.framework.uuid") specifying the UUID for the
 * framework UUID.
 *
 * The framework UUID is used to uniquely identify a single framework. If no framework uuid is provided
 * random uuid will be generated during startup.
 *
 * @note The Celix framework expects framework UUIDs to be unique per process.
 */
#define OSGI_FRAMEWORK_FRAMEWORK_UUID "org.osgi.framework.uuid"

/**
 * @brief Celix framework environment property (named "CELIX_BUNDLES_PATH") which specified a `;` separated
 * list of bundles path used when installing bundles with relative paths.
 *
 * Default value is "bundles".
 *
 * If a Celix framework tries to install a bundle using a relative path it will use the CELIX_BUNDLES_PATH
 * to find the bundle files. This can be seen as a LD_LIBRARY_PATH for bundles.
 */
#define CELIX_BUNDLES_PATH_NAME "CELIX_BUNDLES_PATH"

#define CELIX_BUNDLES_PATH_DEFAULT "bundles"

/**
 * @brief Celix framework environment property (named "CELIX_LOAD_BUNDLES_WITH_NODELETE") which configures if
 * library loaded from bundles should be opened with the RTLD_NODELETE flag.
 *
 * The default value is false.
 *
 * If the value is "true" the RTLD_NODELETE flag will be used to load the shared libraries of bundles.
 * This can be useful for debugging bundles.
 *
 */
#define CELIX_LOAD_BUNDLES_WITH_NODELETE "CELIX_LOAD_BUNDLES_WITH_NODELETE"

/**
 * The path used getting entries from the framework bundle.
 * Normal bundles have an archive directory.
 * For the celix framework by default the working directory is used, with this configuration this can be changed.
 */
#define CELIX_SYSTEM_BUNDLE_ARCHIVE_PATH "CELIX_SYSTEM_BUNDLE_ARCHIVE_PATH"


/**
 * @brief Celix framework environment property (named "CELIX_AUTO_START_0") which specified a (ordered) space
 * separated set of bundles to load and auto start when the Celix framework is started.
 *
 * The Celix framework will first start bundles in for CELIX_AUTO_START_0 and lastly start bundles in CELIX_AUTO_START_6.
 * Bundles which are also started in the order they appear in the AUTO_START set; first bundles mentioned is started
 * first.
 * When the Celix framework stops the bundles are stopped in the reverse order. Bundles in CELIX_AUTO_START_6 are stopped
 * first and of those bundles, the bundle mentioned last in a AUTO_START set is stopped first.
 */
#define CELIX_AUTO_START_0 "CELIX_AUTO_START_0"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_1 "CELIX_AUTO_START_1"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_2 "CELIX_AUTO_START_2"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_3 "CELIX_AUTO_START_3"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_4 "CELIX_AUTO_START_4"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_5 "CELIX_AUTO_START_5"

/**
 * @see CELIX_AUTO_START_0
 */
#define CELIX_AUTO_START_6 "CELIX_AUTO_START_6"


#ifdef __cplusplus
}
#endif

#endif /* CELIX_CONSTANTS_H_ */


