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

QGC_LOGGING_CATEGORY(MessageIvaqManagerLog, "MessageIvaqManagerLog")

#define IVAQ_MVK_TIMEOUT 2000

MessageIvaqManager::MessageIvaqManager(Vehicle* vehicle)
    : QObject               (vehicle)
    , _mavlink              (nullptr)
    , _vehicle              (vehicle)
    , _payloadStatus        (_colorGrey)         
    , _amplificationStage1  (_colorGrey) 
    , _amplificationStage2  (_colorGrey) 
    , _signalSaturation     (_colorGrey) 
    , _signalLevel          (99) // "--"
    , _noiseLevel           (99) //  
    , _saveStatus           (_colorGrey) 
{
    _mavlink = qgcApp()->toolbox()->mavlinkProtocol();

      // Timer to track a healthy RID device. When expired we let the operator know
    _ivaqTimeoutTimer.setSingleShot(true);
    _ivaqTimeoutTimer.setInterval(IVAQ_MVK_TIMEOUT);
    connect(&_ivaqTimeoutTimer, &QTimer::timeout, this, &MessageIvaqManager::_ivaqTimeout);

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
            _handleNamedValueFloat(message);
            break;
        default:
            break;
    }
}

// This slot will be called if we stop receiving mavlink messages for more than IVAQ_MVK_TIMEOUT seconds
void MessageIvaqManager::_ivaqTimeout()
{
    _payloadStatusRaw = POWER_ON_ERROR_COMMS;

    // We need to set PL Status Led to RED 
    _payloadStatus = _colorRed;
    emit payloadStatusChanged();

    // Rest of LEDs need to be set to grey
    // Amp Stages
    _amplificationStage1 =  _colorGrey;
    _amplificationStage2 =  _colorGrey;
    emit amplificationStage1Changed();
    emit amplificationStage2Changed();
    // Saturation Led
    _signalSaturation = _colorGrey;
    emit signalSaturationChanged();
    // Save Status Led
    _saveStatus = _colorGrey;
    emit saveStatusChanged();

    // Signal and Noise Levels shall be set to "--"
    _signalLevel = 99; //"--";
    emit signalLevelChanged();
    _noiseLevel = 99; //"--";
    emit noiseLevelChanged();

    qCDebug(MessageIvaqManagerLog) << "We stopped receiving mavlink messages from Ivaq Payload.";
}

void MessageIvaqManager::_handleNamedValueInt(const mavlink_message_t& message)
{
    mavlink_msg_named_value_int_decode(&message, &namval_mvk_msg);
    _payloadGenParamsDecode(&namval_mvk_msg);
    // Restart the timeout
    _ivaqTimeoutTimer.start();
}

void MessageIvaqManager::_handleNamedValueFloat(const mavlink_message_t& message)
{
    mavlink_msg_named_value_float_decode(&message, &signal_mvk_msg);
    _payloadSignalDecode(&signal_mvk_msg);
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

void MessageIvaqManager::_payloadSignalDecode (mavlink_named_value_float_t *signal_mvk_msg)
{
    if (strcmp(signal_mvk_msg->name,"RX_SIGNAL") == 0)
    {
        _signalLevelRaw = signal_mvk_msg->value;
        _signalLevel = (int)_signalLevelRaw;
        //qDebug() << _signalLevel;
        
        emit signalLevelChanged();
    } 
}
