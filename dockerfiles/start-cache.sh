#!/bin/bash

#  Copyright 2018 U.C. Berkeley RISE Lab
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

cd $HYDRO_HOME/anna-cache
mkdir -p conf

IP=`ifconfig eth0 | grep 'inet addr:' | grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1 }'`

# Download latest version of the code from relevant repository & branch -- if
# none are specified, we use hydro-project/anna by default.
git remote remove origin
if [[ -z "$REPO_ORG" ]]; then
  REPO_ORG="hydro-project"
fi

if [[ -z "$REPO_BRANCH" ]]; then
  REPO_BRANCH="master"
fi

git remote add origin https://github.com/$REPO_ORG/anna-cache
git fetch -p origin
git checkout -b brnch origin/$REPO_BRANCH

cd build && make -j4 && cd ..

# Do not start the server until conf/anna-config.yml has been copied onto this
# pod -- if we start earlier, we won't now how to configure the system.
while [[ ! -f "conf/anna-config.yml" ]]; do
  continue
done


echo -e "user:" >> conf/anna-config.yml
echo -e "    ip: $IP" >> conf/anna-config.yml
echo -e "    routing-elb: $ROUTE_ADDR" >> conf/anna-config.yml

./build/functions/cache/src/flfunc-async-cache
