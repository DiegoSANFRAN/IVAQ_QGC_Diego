/****************************************************************************
 *
 * (c) 2023 IVAQ PROJECT <http://www.ivaq.es>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#ifndef IVAQPAYLOAD_MANAGER_H
#define IVAQPAYLOAD_MANAGER_H

#include <QObject>
#include <QString>
#include <qqml.h>

#include "QGCMAVLink.h"
#include "Vehicle.h"

typedef enum
{
  POWER_OFF             = 0x00U,
  POWER_ON              = 0x01U,
  POWER_ON_LOW_BATT     = 0x02U,
  POWER_ON_ERROR_COMMS  = 0x03U
} RxStatus_TypeDef;

typedef enum
{
  STAGE_1_OFF_STAGE_2_OFF = 0x00U,
  STAGE_1_ON_STAGE_2_OFF = 0x01U,
  STAGE_1_OFF_STAGE_2_ON = 0x02U,
  STAGE_1_ON_STAGE_2_ON = 0x03U,
} SM_StageStates_TypeDef;

typedef enum
{
  SA_FALSE    = 0x00U,
  SA_TRUE     = 0x01U
} SA_BoolTypeDef;

typedef enum
{
  SAVE_FALSE  = 0x00U,
  SAVE_PB_ON  = 0x01U,
  SAVE_ACQ    = 0x02U,
  SAVE_2_SD   = 0x03U
} SV_SaveState_TypeDef;


Q_DECLARE_LOGGING_CATEGORY(RemoteIDManagerLog)

class MessageIvaqManager : public QObject
{
  Q_OBJECT
  
public:
    MessageIvaqManager(Vehicle* vehicle);

    Q_PROPERTY(QString payloadStatus        READ payloadStatus           NOTIFY payloadStatusChanged);
    Q_PROPERTY(QString amplificationStage1  READ amplificationStage1     NOTIFY amplificationStage1Changed);
    Q_PROPERTY(QString amplificationStage2  READ amplificationStage2     NOTIFY amplificationStage2Changed);
    Q_PROPERTY(QString signalSaturation     READ signalSaturation        NOTIFY signalSaturationChanged);
    Q_PROPERTY(int     signalLevel          READ signalLevel             NOTIFY signalLevelChanged);
    Q_PROPERTY(int     noiseLevel           READ noiseLevel              NOTIFY noiseLevelChanged);
    Q_PROPERTY(QString saveStatus           READ saveStatus              NOTIFY saveStatusChanged);

    QString payloadStatus         (void) const { return _payloadStatus; }
    QString amplificationStage1   (void) const { return _amplificationStage1; }
    QString amplificationStage2   (void) const { return _amplificationStage2; }
    QString signalSaturation      (void) const { return _signalSaturation; }
    int signalLevel               (void) const { return _signalLevel; }
    int noiseLevel                (void) const { return _noiseLevel; }
    QString saveStatus            (void) const { return _saveStatus; }

    void mavlinkMessageReceived (mavlink_message_t& message);
    
signals:
    void payloadStatusChanged();
    void amplificationStage1Changed();
    void amplificationStage2Changed();
    void signalSaturationChanged();
    void signalLevelChanged();
    void noiseLevelChanged();
    void saveStatusChanged();

private slots:
    void _ivaqTimeout();

private:

    QString _colorGrey          = "#808080";
    QString _colorYellow        = "#b95604";
    QString _colorRed           = "#ed3939";
    QString _colorGreen         = "#009431";
    QString _colorBlue          = "#1a72ff";

    RxStatus_TypeDef        _payloadStatusRaw;
    SM_StageStates_TypeDef  _amplificationStagesRaw;
    SA_BoolTypeDef          _signalSaturationRaw;
    SV_SaveState_TypeDef    _saveStatusRaw;

    float_t               _signalLevelRaw;
    char                  _signalLevelRaw_str[4];
    mavlink_named_value_int_t namval_mvk_msg;
    mavlink_named_value_float_t signal_mvk_msg;

    MAVLinkProtocol*    _mavlink;
    Vehicle*            _vehicle;

    QString _payloadStatus;         
    QString _amplificationStage1;   
    QString _amplificationStage2;   
    QString _signalSaturation;      
    int     _signalLevel;         
    int     _noiseLevel;            
    QString _saveStatus;     

    // IVAQ - Management of PL Variables through NAMED_VALUE_INT and NAMED_VALUE_FLOAT mavlink messages 
    void _handleNamedValueInt           (const mavlink_message_t& message);
    void _handleNamedValueFloat         (const mavlink_message_t& message);

    void _payloadGenParamsDecode (mavlink_named_value_int_t *namval_mvk_msg);
    void _payloadSignalDecode (mavlink_named_value_float_t *signal_mvk_msg);

    // Timer that checks if Payload is sending messages
    QTimer _ivaqTimeoutTimer;
};

#endif