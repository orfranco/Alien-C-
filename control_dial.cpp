#include "control_dial.h"
#include <QVBoxLayout>

int ControlDial::nextControlChangeNumber = 100;

ControlDial::ControlDial(const QString& label, QWidget* parent)
    : QWidget(parent), m_midiCC(nextControlChangeNumber++), m_sendEnabled(true)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_dial = new QDial(this);
    m_dial->setRange(0, 127);
    m_dial->setValue(64);
    m_dial->setFixedSize(120, 120);
    m_dial->setNotchesVisible(true);

    m_label = new QLabel(QString("%1 MIDI Value: 64").arg(label), this);

    m_sendToggle = new QCheckBox(QString("Send %1").arg(label), this);
    m_sendOnlyButton = new QPushButton(QString("Send Only %1").arg(label), this);

    m_midiCCBox = new QSpinBox(this);
    m_midiCCBox->setRange(0, 127); // MIDI CC values are from 0 to 127
    m_midiCCBox->setValue(m_midiCC);
    m_midiCCBox->setPrefix("MIDI CC: ");

    layout->addWidget(m_dial);
    layout->addWidget(m_label);
    layout->addWidget(m_sendToggle);
    layout->addWidget(m_sendOnlyButton);
    layout->addWidget(m_midiCCBox);  // Add the MIDI CC box to the layout

    setToggleStyle();
    toggleSendStateWithParam(true);

    connect(m_sendToggle, &QCheckBox::stateChanged, this, &ControlDial::toggleSendState);
    connect(m_midiCCBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ControlDial::onMidiCCChanged);
}

void ControlDial::setDialValue(int value)
{
    if (m_sendEnabled)
        emit sendMidiMessage(value, m_midiCC);

    m_dial->setValue(value);
    m_label->setText(QString("MIDI Value: %1").arg(value));
}

bool ControlDial::isSendEnabled() const
{
    return m_sendEnabled;
}

void ControlDial::toggleSendState()
{
    m_sendEnabled = m_sendToggle->isChecked();
}

void ControlDial::toggleSendStateWithParam(bool isChecked)
{
    m_sendToggle->setChecked(isChecked);
}

void ControlDial::onMidiCCChanged(int newCC)
{
    m_midiCC = newCC;
}

void ControlDial::setToggleStyle()
{
    m_sendToggle->setStyleSheet(
        "QCheckBox::indicator { width: 50px; height: 25px; }"
        "QCheckBox::indicator:checked { background-color: green; border-radius: 12px; }"
        "QCheckBox::indicator:unchecked { background-color: red; border-radius: 12px; }"
    );
}
