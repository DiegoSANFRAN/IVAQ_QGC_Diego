/****************************************************************************
 *
 * (c) 2023 IVAQ QGROUNDCONTROL PROJECT <http://www.ivaq.es>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.12
import QtQuick.Layouts  1.12

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette       1.0

Rectangle {
    id:                 visualIvaqInstrument
    height:             parent.width //_outerRadius * 2
    Layout.fillWidth:   true
    radius:             _outerRadius
    color:              qgcPal.window

    //id:         root

    property real   _innerRadius:           (width - (_topBottomMargin * 3)) / 4
    property real   _outerRadius:           parent.width *0.05 //_innerRadius + _topBottomMargin
    property real   _spacing:               40 //ScreenTools.defaultFontPixelHeight * 0.33
    property real   _topBottomMargin:       (width * 0.05) / 2

    // General properties
    property var  _activeVehicle:           QGroundControl.multiVehicleManager.activeVehicle
    
    QGCPalette { id: qgcPal }

    DeadMouseArea { anchors.fill: parent }

    // PL Status
    QGCLabel {
        id:                      plStatus
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Payload Status"
    }

    // PL Status LED
    Rectangle {
        id:                      plStatusLed
        height:                  parent.width / 15 //_outerRadius * 2
        width:                   height
        Layout.fillWidth:        true
        radius:                  _outerRadius / 10
        anchors.verticalCenter:  plStatus.verticalCenter
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 3

        color:                   _activeVehicle.messageIvaqManager.payloadStatus
    }

    // Amplif. Stages
    QGCLabel {
        id:                      plAmplification
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Amplification Stages"
    }
    
    // Amplif. Stage 1 LED
    Rectangle {
        id:                      plAmplificationLed
        height:                  parent.width / 15 //_outerRadius * 2
        width:                   height
        Layout.fillWidth:        true
        radius:                  _outerRadius / 10
        anchors.verticalCenter:  plAmplification.verticalCenter
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 3

        color:                   _activeVehicle.messageIvaqManager.amplificationStage1
    }

    // Amplif. Stage 2 LED
    Rectangle {
        id:                      plAmplificationLed2
        height:                  parent.width / 15 //_outerRadius * 2
        width:                   height
        Layout.fillWidth:        true
        radius:                  _outerRadius / 10
        anchors.verticalCenter:  plAmplification.verticalCenter
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 1

        color:                   _activeVehicle.messageIvaqManager.amplificationStage2
    }

    // Signal Saturation
    QGCLabel {
        id:                      plSignalSat
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 2
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        //anchors.horizontal:      parent.horizontal
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Signal Saturation"
    }

    // Signal Saturation LED
    Rectangle {
        id:                      plSignalSatLed
        height:                  parent.width / 15 //_outerRadius * 2
        width:                   height
        Layout.fillWidth:        true
        radius:                  _outerRadius / 10
        anchors.verticalCenter:  plSignalSat.verticalCenter
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 3

        color:                   _activeVehicle.messageIvaqManager.signalSaturation
    }

    // Signal Level
    QGCLabel {
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 3
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        //anchors.horizontal:      parent.horizontal
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Signal Level"
    }

    // Signal Level Text
    Rectangle {
        id:                      plSignalLevel
        height:                  parent.width / 10 //_outerRadius * 2
        width:                   height * 1.5
        Layout.fillWidth:        true
        radius:                  _outerRadius / 5
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 3
        anchors.top:             parent.top
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 1.75

        color:                   qgcPal.window //qgcPal.window
        border.color:            qgcPal.mapWidgetBorderLight
        
        QGCLabel {
            id:                         plSignalLevelText
            text:                       _plSigLevelString
            anchors.horizontalCenter:   parent.horizontalCenter
            anchors.verticalCenter:     parent.verticalCenter

            property string _plSigLevelString:  "--"
        }
        QGCLabel {
            anchors.left:               plSignalLevel.right
            anchors.leftMargin:         4

            text:                       "dB"
            font.pointSize:             12
        }
    }

    // Noise Level
    QGCLabel {
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 4
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        //anchors.horizontal:      parent.horizontal
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Noise Level"
    }

    // Noise Level Text
    Rectangle {
        id:                      plNoiseLevel
        height:                  parent.width / 10 //_outerRadius * 2
        width:                   height * 1.5
        Layout.fillWidth:        true
        radius:                  _outerRadius / 5
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 4
        anchors.top:             parent.top
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 1.75

        color:                   qgcPal.window //qgcPal.window
        border.color:            qgcPal.mapWidgetBorderLight
        
        QGCLabel {
            id:                         plNoiseLevelText
            text:                       _plNoiseLevelString
            anchors.horizontalCenter:   parent.horizontalCenter
            anchors.verticalCenter:     parent.verticalCenter

            property string _plNoiseLevelString:  "--"
        }
        QGCLabel {
            anchors.left:               plNoiseLevel.right
            anchors.leftMargin:         4

            text:                       "dB"
            font.pointSize:             12
        }
    }

    // Save Status
    QGCLabel {
        id:                      plSaveStatus
        anchors.topMargin:       Math.round(ScreenTools.defaultFontPixelHeight) * 1.5 + _spacing * 5
        anchors.top:             parent.top
        anchors.left:            parent.left
        anchors.leftMargin:      Math.round(ScreenTools.defaultFontPixelHeight)
        //anchors.horizontal:      parent.horizontal
        text:                    _plString
        font.pointSize:          12
        color:                   "white"
        

        property string _plString: "Save Status"
    }

    // Save Status LED
    Rectangle {
        id:                      plSaveStatusLed
        height:                  parent.width / 15 //_outerRadius * 2
        width:                   height
        Layout.fillWidth:        true
        radius:                  _outerRadius / 10
        anchors.verticalCenter:  plSaveStatus.verticalCenter
        anchors.right:           parent.right
        anchors.rightMargin:     Math.round(ScreenTools.defaultFontPixelHeight) * 3

        color:                   _activeVehicle.messageIvaqManager.saveStatus
    }
}

