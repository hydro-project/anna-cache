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

#ifndef INCLUDE_CACHE_THREADS_HPP
#define INCLUDE_CACHE_THREADS_HPP

#include "threads.hpp"
#include "types.hpp"

const unsigned kCausalCacheUpdatePort = 7150;
const unsigned kCausalCacheVersionGCPort = 7200;
const unsigned kCausalCacheVersionedKeyRequestPort = 7250;
const unsigned kCausalCacheVersionedKeyResponsePort = 7300;

class CausalCacheThread {
  Address ip_;
  Address ip_base_;
  unsigned tid_;

public:
  CausalCacheThread(Address ip, unsigned tid)
      : ip_(ip), ip_base_("tcp://" + ip_ + ":"), tid_(tid) {}

  Address ip() const { return ip_; }

  unsigned tid() const { return tid_; }

  Address causal_cache_get_bind_address() const {
    return "ipc:///requests/get";
  }

  Address causal_cache_get_connect_address() const {
    return "ipc:///requests/get";
  }

  Address causal_cache_put_bind_address() const {
    return "ipc:///requests/put";
  }

  Address causal_cache_put_connect_address() const {
    return "ipc:///requests/put";
  }

  Address causal_cache_update_bind_address() const {
    return kBindBase + std::to_string(tid_ + kCausalCacheUpdatePort);
  }

  Address causal_cache_update_connect_address() const {
    return ip_base_ + std::to_string(tid_ + kCausalCacheUpdatePort);
  }

  Address causal_cache_version_gc_bind_address() const {
    return kBindBase + std::to_string(tid_ + kCausalCacheVersionGCPort);
  }

  Address causal_cache_version_gc_connect_address() const {
    return ip_base_ + std::to_string(tid_ + kCausalCacheVersionGCPort);
  }

  Address causal_cache_key_version_request_bind_address() const {
    return kBindBase +
           std::to_string(tid_ + kCausalCacheVersionedKeyRequestPort);
  }

  Address causal_cache_key_version_request_connect_address() const {
    return ip_base_ +
           std::to_string(tid_ + kCausalCacheVersionedKeyRequestPort);
  }

  Address causal_cache_key_version_response_bind_address() const {
    return kBindBase +
           std::to_string(tid_ + kCausalCacheVersionedKeyResponsePort);
  }

  Address causal_cache_key_version_response_connect_address() const {
    return ip_base_ +
           std::to_string(tid_ + kCausalCacheVersionedKeyResponsePort);
  }
};

#endif // INCLUDE_CACHE_THREADS_HPP
