ecmwf-api-client
================

Installation
============

Install via pip with:

> $ pip install ecmwf-api-client

Configure
=========

* If you don't have an ECMWF account, please self register at https://apps.ecmwf.int/registration/ and then go to the steps below.
* Login https://apps.ecmwf.int/auth/login/
* Retrieve you key at https://api.ecmwf.int/v1/key/

Note that the key expires in 1 year. You will receive an email to the registered email address 1 month before the expiration date with the renewal instructions. To check the expiry date of your current key log into www.ecmwf.int, and go to https://api.ecmwf.int/v1/key/.

* Copy the information in this page and paste it in the file $HOME/.ecmwfapirc (Unix/Linux) or %USERPROFILE%\.ecmwfapirc (Windows: usually in C:\Users\<USERNAME>\.ecmwfapirc ; see how to create a file with a leading dot).

Your $HOME/.ecmwfapirc (Unix/Linux) or %USERPROFILE%\.ecmwfapirc (Windows) should look something like this:
```
{
    "url"   : "https://api.ecmwf.int/v1",
    "key"   : "XXXXXXXXXXXXXXXXXXXXXX",
    "email" : "john.smith@example.com"
}
```
* You can browse the ECMWF data catalogue at https://apps.ecmwf.int/mars-catalogue/

Test
====

You can test this small python script to retrieve TIGGE data:
```
#!/usr/bin/env python
from ecmwfapi import ECMWFDataServer

# To run this example, you need an API key 
# available from https://api.ecmwf.int/v1/key/

server = ECMWFDataServer()    
server.retrieve({
    'origin'    : "ecmf",
    'levtype'   : "sfc",
    'number'    : "1",
    'expver'    : "prod",
    'dataset'   : "tigge",
    'step'      : "0/6/12/18",
    'area'      : "70/-130/30/-60",
    'grid'      : "2/2",
    'param'     : "167",
    'time'      : "00/12",
    'date'      : "2014-11-01",
    'type'      : "pf",
    'class'     : "ti",
    'target'    : "tigge_2014-11-01_0012.grib"
})
```

License
=======

Copyright 2019 European Centre for Medium-Range Weather Forecasts (ECMWF)
Licensed under the Apache License, Version 2.0 (the “License”); you may not use this file except in compliance with the License. You may obtain a copy of the License at

> http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
In applying this licence, ECMWF does not waive the privileges and immunities granted to it by virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
