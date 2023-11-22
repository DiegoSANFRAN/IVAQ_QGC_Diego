/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "IVAQPayloadManager.h"
#include "QGCApplication.h"

#include <QDebug>

MessageIvaqManager::MessageIvaqManager(Vehicle* vehicle)
    : QObject               (vehicle)
    , _mavlink              (nullptr)
    , _vehicle              (vehicle)
    , _payloadStatus        (_colorGrey)         
    , _amplificationStage1  (_colorGrey) 
    , _amplificationStage2  (_colorGrey) 
    , _signalSaturation     (_colorGrey) 
    , _signalLevel          (_colorGrey) 
    , _noiseLevel           (_colorGrey) 
    , _saveStatus           (_colorGrey) 
{
    _mavlink = qgcApp()->toolbox()->mavlinkProtocol();
}

void MessageIvaqManager::mavlinkMessageReceived(mavlink_message_t& message )
{
    switch (message.msgid) 
    {
        // IVAQ - Management of PL Variables through NAMED_VALUE_INT and NAMED_VALUE_FLOAT mavlink messages 
        case MAVLINK_MSG_ID_NAMED_VALUE_INT:
            _handleNamedValueInt(message);
            break;
        case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT:
            //_handleNamedValueFloat(message);
            break;
        default:
            break;
    }
}

void MessageIvaqManager::_handleNamedValueInt(const mavlink_message_t& message)
{
    mavlink_named_value_int_t namval_mvk_msg;

    mavlink_msg_named_value_int_decode(&message, &namval_mvk_msg);
    
    _payloadGenParamsDecode(&namval_mvk_msg);
}

void MessageIvaqManager::_payloadGenParamsDecode (mavlink_named_value_int_t *namval_mvk_msg)
{
    if (strcmp(namval_mvk_msg->name,"RX_STATE") == 0)
    {
        _payloadStatusRaw = (RxStatus_TypeDef)namval_mvk_msg->value;
        //qDebug() << "Estoy en el if que quiero" << _payloadStatusRaw << namval_mvk_msg->value;
        switch (_payloadStatusRaw)
        {
            case POWER_OFF:
                _payloadStatus = _colorGrey;
                break;
            case POWER_ON:
                _payloadStatus = _colorGreen;
                break;
            case POWER_ON_LOW_BATT:
                _payloadStatus = _colorRed;
                break;
            case POWER_ON_ERROR_COMMS:
                _payloadStatus = _colorRed;
                break;
        }
        emit payloadStatusChanged();
    } 
    else if (strcmp(namval_mvk_msg->name,"RX_STGS") == 0)
    {
        _amplificationStagesRaw = (SM_StageStates_TypeDef)namval_mvk_msg->value;
        switch (_amplificationStagesRaw)
        {
            case STAGE_1_OFF_STAGE_2_OFF:
                _amplificationStage1 =  _colorGrey;
                _amplificationStage2 =  _colorGrey;
                break;
            case STAGE_1_ON_STAGE_2_OFF:
                _amplificationStage1 =  _colorBlue;
                _amplificationStage2 =  _colorGrey;
                break;
            case STAGE_1_OFF_STAGE_2_ON:
                _amplificationStage1 =  _colorGrey;
                _amplificationStage2 =  _colorBlue;
                break;
            case STAGE_1_ON_STAGE_2_ON:
                _amplificationStage1 =  _colorBlue;
                _amplificationStage2 =  _colorBlue;
                break;
        }
        emit amplificationStage1Changed();
        emit amplificationStage2Changed();
    }
    else if (strcmp(namval_mvk_msg->name, "RX_SATUR") == 0)
    {
        _signalSaturationRaw = (SA_BoolTypeDef)namval_mvk_msg->value;
        switch (_signalSaturationRaw)
        {
            case SA_FALSE:
                _signalSaturation = _colorGreen;
                break;
            case SA_TRUE:
                _signalSaturation = _colorRed;
                break;
        }
        emit signalSaturationChanged();
    }
    else if (strcmp(namval_mvk_msg->name,"RX_SAVE") == 0)
    {
        _saveStatusRaw = (SV_SaveState_TypeDef)namval_mvk_msg->value;
        switch (_saveStatusRaw)
        {
            case SAVE_FALSE:
                _saveStatus = _colorGrey;
                break;
            case SAVE_PB_ON:
                _saveStatus = _colorYellow;
                break;
            case SAVE_ACQ:
                _saveStatus = _colorYellow;
                break;
            case SAVE_2_SD:
                _saveStatus = _colorYellow;
                break;
        }
        emit saveStatusChanged();
    }
}

void MessageIvaqManager::_payloadSigParamsDecode (mavlink_named_value_float_t *signal_mvk_msg)
{

}
