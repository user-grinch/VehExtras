#include "pch.h"
#include "wheelhub.h"

void UpdateRotation(RwFrame* ori, RwFrame* tar) {
    if (ori && tar) {
        // pos.y -= 0.02f;
        // RwFrameGetMatrix(tar)->pos = pos;
        float oriRot = Util::GetMatrixRotationZ(&ori->modelling);
        Util::SetMatrixRotationZ(&tar->modelling, oriRot);
    }
}

void WheelHub::FindNodes(RwFrame* frame, void* ptr) {
    if (frame) {
        CVehicle* pVeh = static_cast<CVehicle*>(ptr);
        std::string name = GetFrameNodeName(frame);

        VehData& data = xData.Get(pVeh);
        std::smatch match;
        if (std::regex_search(name, match, std::regex("wheel_([a-zA-Z]{2})_dummy"))) {
            std::string str = match[1].str();
            char first = std::tolower(str[0]);
            char second = std::tolower(str[1]);

            if (first == 'r') {
                if (second == 'f') {
                    data.wheelrf = frame;
                }
                else if (second == 'm') {
                    data.wheelrm = frame;
                }
                else if (second == 'b') {
                    data.wheelrb = frame;
                }
            }
            else if (first == 'l') {
                if (second == 'f') {
                    data.wheellf = frame;
                }
                else if (second == 'm') {
                    data.wheellm = frame;
                }
                else if (second == 'b') {
                    data.wheellb = frame;
                }
            }
        }

        if (std::regex_search(name, match, std::regex("hub_([a-zA-Z]{2})"))) {
            std::string str = match[1].str();
            char first = std::tolower(str[0]);
            char second = std::tolower(str[1]);

            if (first == 'r') {
                if (second == 'f') {
                    data.hubrf = frame;
                }
                else if (second == 'm') {
                    data.hubrm = frame;
                }
                else if (second == 'b') {
                    data.hubrb = frame;
                }
            }
            else if (first == 'l') {
                if (second == 'f') {
                    data.hublf = frame;
                }
                else if (second == 'm') {
                    data.hublm = frame;
                }
                else if (second == 'b') {
                    data.hublb = frame;
                }
            }
        }

        if (RwFrame* newFrame = frame->child) {
            FindNodes(newFrame, ptr);
        }
        if (RwFrame* newFrame = frame->next) {
            FindNodes(newFrame, ptr);
        }
    }
    return;
}

void WheelHub::Process(void* ptr, RwFrame* frame, eModelEntityType type) {
    CVehicle* pVeh = static_cast<CVehicle*>(ptr);

    VehData& data = xData.Get(pVeh);
    if (!data.m_bInit) {
        FindNodes(frame, ptr);
        data.m_bInit = true;
    }

    UpdateRotation(data.wheellf, data.hublf);
    UpdateRotation(data.wheellm, data.hublm);
    UpdateRotation(data.wheellb, data.hublb);
    UpdateRotation(data.wheelrf, data.hubrf);
    UpdateRotation(data.wheelrm, data.hubrm);
    UpdateRotation(data.wheelrb, data.hubrb);
}