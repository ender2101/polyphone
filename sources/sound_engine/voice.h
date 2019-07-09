/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2019 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program. If not, see http://www.gnu.org/licenses/.    **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: https://www.polyphone-soundfonts.com                 **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef VOICE_H
#define VOICE_H

#include <QMutex>
#include "sound.h"
#include "enveloppevol.h"
#include "oscsinus.h"
#include "Chorus.h"
#include "FreeVerb.h"

class Voice : public QObject
{
    Q_OBJECT

public:
    Voice(QByteArray baData, quint32 smplRate, quint32 audioSmplRate, int note, int velocity, VoiceParam *voiceParam);
    ~Voice();

    int getNote() { return _note; }
    void release(bool quick = false);
    void setGain(double gain);
    void setChorus(int level, int depth, int frequency);
    bool isFinished() { return _isFinished; }
    bool isRunning() { return _isRunning; }
    void runVoice(quint32 delay) { _isRunning = true; _delayStart = delay; }

    // Accès aux propriétés de voiceParam
    double getPan();
    int getExclusiveClass();
    int getPresetNumber();
    float getReverb();

    // Modification des propriétés de voiceParam
    void setPan(double val);
    void setLoopMode(int val);
    void setLoopStart(quint32 val);
    void setLoopEnd(quint32 val);
    void setFineTune(qint32 val);

    // Generate data
    void generateData(float *dataL, float *dataR, quint32 len);

signals:
    void currentPosChanged(quint32 pos);

private:
    // Oscillateurs, enveloppes et chorus
    OscSinus _modLFO, _vibLFO;
    EnveloppeVol _enveloppeVol, _enveloppeMod;
    stk::Chorus _chorus;
    int _chorusLevel;

    // Données son et paramètres
    QByteArray _baData;
    quint32 _smplRate, _audioSmplRate;
    int _note;
    int _velocity;
    double _gain;
    VoiceParam * _voiceParam;

    // Lecture du sample
    quint32 _currentSmplPos;
    double _time;
    bool _release;
    quint32 _delayEnd, _delayStart;
    bool _isFinished;
    bool _isRunning;

    // Save state for resampling
    float _deltaPos;
    qint32 _valPrec, _valBase;

    // Save state for low pass filter
    double _x1, _x2, _y1, _y2;

    bool takeData(qint32 *data, quint32 nbRead);
    void biQuadCoefficients(double &a0, double &a1, double &a2, double &b1, double &b2, double freq, double Q);

    // protection des paramètres
    QMutex _mutexParam;
};

#endif // VOICE_H
