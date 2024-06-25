#!/bin/bash

for i in {1..30}
do
   /code/easy_kvstore/bin/kvstore_client 192.168.146.8 9999 limittest &
done