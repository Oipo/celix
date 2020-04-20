/**
 *Licensed to the Apache Software Foundation (ASF) under one
 *or more contributor license agreements.  See the NOTICE file
 *distributed with this work for additional information
 *regarding copyright ownership.  The ASF licenses this file
 *to you under the Apache License, Version 2.0 (the
 *"License"); you may not use this file except in compliance
 *with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing,
 *software distributed under the License is distributed on an
 *"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 *specific language governing permissions and limitations
 *under the License.
 */

#ifndef CELIX_PUBSUB_SERIALIZER_HANDLER_H
#define CELIX_PUBSUB_SERIALIZER_HANDLER_H

#include <stdint.h>

#include "celix_api.h"
#include "pubsub_message_serialization_service.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct pubsub_serializer_handler pubsub_serializer_handler_t; //opaque type


/**
 * Creates a handler which track pubsub_custom_msg_serialization_service services with a (serialization.type=<serializerType)) filter.
 * If multiple pubsub_message_serialization_service for the same msg fqn (targeted.msg.fqn property) the highest ranking service will be used.
 *
 * The message handler assumes (and checks) that all provided serialization services do not clash in message ids (so every msgId should have its own msgFqn)
 * and that only one version for a message serialization is registered.
 * This means that all bundles in a single celix container (a single process) should all use the same version of messages.
 *
 * If backwards compatibility is supported, when serialized message with a higher minor version when available in the serializer handler are used to
 * deserialize. This could be supported for serialization like json.
 * So when a json message of version 1.1.x with content {"field1":"value1", "field2":"value2"} is deserialized to a version 1.0 (which only has field1),
 * the message can and will be deserialized
 *
 * @param ctx                   The bundle contest.
 * @param serializerType        type of serialization services to handle (e.g. json, avrobin, etc)
 * @param backwardCompatible    Whether backwards compatible serialization is supported.
 * @return A newly created pubsub serializer handler.
 */
pubsub_serializer_handler_t* pubsub_serializerHandler_create(celix_bundle_context_t* ctx, const char* serializerType, bool backwardCompatible);


void pubsub_serializerHandler_destroy(pubsub_serializer_handler_t* handler);

void pubsub_serializerHandler_addSerializationService(pubsub_serializer_handler_t* handler, pubsub_message_serialization_service_t* svc, const celix_properties_t* svcProperties);
void pubsub_serializerHandler_removeSerializationService(pubsub_serializer_handler_t* handler, pubsub_message_serialization_service_t* svc, const celix_properties_t* svcProperties);

celix_status_t pubsub_serializerHandler_serialize(pubsub_serializer_handler_t* handler, uint32_t msgId, const void* input, struct iovec** output, size_t* outputIovLen);
celix_status_t pubsub_serializerHandler_freeSerializedMsg(pubsub_serializer_handler_t* handler, uint32_t msgId, struct iovec* input, size_t inputIovLen);
celix_status_t pubsub_serializerHandler_deserialize(pubsub_serializer_handler_t* handler, uint32_t msgId, int serializedMajorVersion, int serializedMinorVersion, const struct iovec* input, size_t inputIovLen, void** out);
celix_status_t pubsub_serializerHandler_freeDeserializedMsg(pubsub_serializer_handler_t* handler, uint32_t msgId, void* msg);

bool pubsub_serializerHandler_supportMsg(pubsub_serializer_handler_t* handler, uint32_t msgId, int majorVersion, int minorVersion);
char* pubsub_serializerHandler_getMsgFqn(pubsub_serializer_handler_t* handler, uint32_t msgId);
uint32_t pubsub_serializerHandler_getMsgId(pubsub_serializer_handler_t* handler, const char* msgFqn);
size_t pubsub_serializerHandler_messageSerializationServiceCount(pubsub_serializer_handler_t* handler);

#ifdef __cplusplus
}
#endif

#endif //CELIX_PUBSUB_SERIALIZER_HANDLER_H