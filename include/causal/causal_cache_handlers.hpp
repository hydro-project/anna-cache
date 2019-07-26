//  Copyright 2019 U.C. Berkeley RISE Lab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef FUNCTIONS_CACHE_INCLUDE_CAUSAL_CACHE_HANDLERS_HPP_
#define FUNCTIONS_CACHE_INCLUDE_CAUSAL_CACHE_HANDLERS_HPP_

#include "cache_threads.hpp"
#include "causal_cache_utils.hpp"

void get_request_handler(
    const string &serialized, set<Key> &key_set, StoreType &unmerged_store,
    InPreparationType &in_preparation, StoreType &causal_cut_store,
    VersionStoreType &version_store,
    map<Key, set<Address>> &single_key_callback_map,
    map<Address, PendingClientMetadata> &pending_single_key_metadata,
    map<Address, PendingClientMetadata> &pending_multi_key_metadata,
    map<Key, set<Key>> &to_fetch_map,
    map<Key, std::unordered_map<VectorClock, set<Key>, VectorClockHash>>
        &cover_map,
    SocketCache &pushers, KvsClientInterface *client, logger log,
    const CausalCacheThread &cct,
    map<string, set<Address>> &client_id_to_address_map);

void put_request_handler(const string &serialized, StoreType &unmerged_store,
                         StoreType &causal_cut_store,
                         VersionStoreType &version_store,
                         map<string, Address> &request_id_to_address_map,
                         KvsClientInterface *client, logger log);

void key_version_request_handler(const string &serialized,
                                 VersionStoreType &version_store,
                                 SocketCache &pushers, logger log,
                                 ZmqUtilInterface *kZmqUtil);

void key_version_response_handler(
    const string &serialized, StoreType &causal_cut_store,
    VersionStoreType &version_store,
    map<Address, PendingClientMetadata> &pending_multi_key_metadata,
    map<string, set<Address>> &client_id_to_address_map,
    const CausalCacheThread &cct, SocketCache &pushers,
    ZmqUtilInterface *kZmqUtil, logger log);

void kvs_response_handler(
    const KeyResponse &response, StoreType &unmerged_store,
    InPreparationType &in_preparation, StoreType &causal_cut_store,
    VersionStoreType &version_store,
    map<Key, set<Address>> &single_key_callback_map,
    map<Address, PendingClientMetadata> &pending_single_key_metadata,
    map<Address, PendingClientMetadata> &pending_multi_key_metadata,
    map<Key, set<Key>> &to_fetch_map,
    map<Key, std::unordered_map<VectorClock, set<Key>, VectorClockHash>>
        &cover_map,
    SocketCache &pushers, KvsClientInterface *client, logger log,
    const CausalCacheThread &cct,
    map<string, set<Address>> &get_client_id_to_address_map,
    map<string, Address> &request_id_to_address_map);

void periodic_migration_handler(
    const StoreType &unmerged_store, InPreparationType &in_preparation,
    StoreType &causal_cut_store, VersionStoreType &version_store,
    map<Address, PendingClientMetadata> &pending_multi_key_metadata,
    map<Key, set<Key>> &to_fetch_map,
    map<Key, std::unordered_map<VectorClock, set<Key>, VectorClockHash>>
        &cover_map,
    SocketCache &pushers, KvsClientInterface *client,
    const CausalCacheThread &cct,
    map<string, set<Address>> &client_id_to_address_map, logger log);

// TODO(vikram): Fix header guards
#endif // FUNCTIONS_CACHE_INCLUDE_CAUSAL_CACHE_HANDLERS_HPP_
