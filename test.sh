#!/usr/bin/env bash

base64 /dev/urandom | head -c 9728 > src/load && make test