#!/usr/bin/env bash

base64 /dev/urandom | head -c 400000 > src/load && make test