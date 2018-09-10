/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "UBLOX_PPP.h"
#include "UBLOX_PPP_CellularNetwork.h"
#include "UBLOX_PPP_CellularPower.h"
#include "UBLOX_PPP_Module.h"

#include "CellularLog.h"

using namespace mbed;
using namespace events;

UBLOX_PPP::UBLOX_PPP(EventQueue &queue) : AT_CellularDevice(queue)
{
}

UBLOX_PPP::~UBLOX_PPP()
{
}

CellularNetwork *UBLOX_PPP::open_network(FileHandle *fh)
{
    if (!_network) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _network = new UBLOX_PPP_CellularNetwork(*atHandler);
            if (!_network) {
                release_at_handler(atHandler);
            }
        }
    }
    return _network;
}

CellularPower *UBLOX_PPP::open_power(FileHandle *fh)
{
    if (!_power) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _power = new UBLOX_PPP_CellularPower(*get_at_handler(fh));
            if (!_power) {
                release_at_handler(atHandler);
            }
        }
    }
    return _power;
}

nsapi_error_t UBLOX_PPP::init_module(FileHandle *fh)
{
    CellularInformation *information = open_information(fh);
    if (!information) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    char model[sizeof("SARA-R4") + 1]; // sizeof need to be long enough to hold just the model text
    nsapi_error_t ret = information->get_model(model, sizeof(model));
    close_information();
    if (ret != NSAPI_ERROR_OK) {
        tr_error("Cellular model not found!");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return UBLOX_PPP_Module::detect_model(model);
}
