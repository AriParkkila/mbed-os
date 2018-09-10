/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include <string.h>

#include "AT_CellularBase.h"
#include "CellularLog.h"
#include "UBLOX_PPP_Module.h"

using namespace mbed;

// unsupported features as per SARA-R4/N4 series, AT Commands Manual, UBX-17003787
static const AT_CellularBase::SupportedFeature unsupported_features_sara_r4[] =  {
    AT_CellularBase::AT_CGDATA, // not supported as per SARA-R4/N4 series, AT Commands Manual, UBX-17003787
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

UBLOX_PPP_Module::Model UBLOX_PPP_Module::_model;

nsapi_error_t UBLOX_PPP_Module::detect_model(const char *model)
{
    static const AT_CellularBase::SupportedFeature *unsupported_features;
    if (memcmp(model, "SARA-R4", sizeof("SARA-R4") - 1) == 0) {
        _model = ModelSaraR4;
        unsupported_features = unsupported_features_sara_r4;
    } else {
        return NSAPI_ERROR_OK;
    }
    tr_info("Cellular model %s (%d)", model, _model);
    AT_CellularBase::set_unsupported_features(unsupported_features);
    return NSAPI_ERROR_OK;
}

UBLOX_PPP_Module::Model UBLOX_PPP_Module::get_model()
{
    return _model;
}
