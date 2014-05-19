#!/bin/bash

# help for https://gist.github.com/chrisjacob/833223
gitbook build
cp -rf _book/* ../securitybook-pages/
rm -rf _book
