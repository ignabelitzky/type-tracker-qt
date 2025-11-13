#include "typetracker.h"
#include "ui_typetracker.h"
#include <QTimer>
#include <QMessageBox>

BaseTypeTracker::BaseTypeTracker() :
    m_wordIndex{0}, m_error{0}, m_amount{0},
    m_chars{0}, m_pastError{false}
{
    m_words = {"love", "friendship", "peace", "enjoy", "hope", "courage", "honor", "wisdom", "happiness", "freedom", "respect", "justice", "solidarity", "gratitude", "resilience", "compassion", "kindness", "optimism", "creativity", "determination", "patience", "humility", "integrity", "trust", "forgiveness", "self-love", "responsibility", "discipline", "tolerance", "enthusiasm", "diligence", "altruism", "loyalty", "sensitivity", "adaptability", "assertiveness", "competence", "consistency", "collaboration", "empathy", "balance", "fidelity", "strength", "honesty", "inclusion", "innovation", "inspiration", "modesty", "objectivity", "perseverance", "insight", "prudence", "rationality", "resolution", "simplicity", "sincerity", "transparency", "unity", "bravery", "vigilance", "vivacity", "zeal", "shelter", "hospitality", "admiration", "relief", "benevolence", "calmness", "clarity", "companionship", "commitment", "understanding", "comfort", "consideration", "contentment", "care", "dedication", "delicacy", "dignity", "discretion", "thoughtfulness", "generosity", "joyfulness", "peacefulness", "serenity", "compassionate", "benevolent", "sincere", "truthful", "mindfulness", "carefulness", "attentiveness", "warmth", "humbleness", "gentleness", "charity", "kindhearted", "helpfulness", "understanding", "tenderness", "loyal", "friendly", "cheerful", "energetic", "enthusiastic"};
}

BaseTypeTracker::~BaseTypeTracker() {}

TypeTracker::TypeTracker(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TypeTracker), m_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->counterTimeEdit->setDisplayFormat("ss");
    connect(m_timer, &QTimer::timeout, this, &TypeTracker::updateCounter);
    connect(ui->startButton, &QPushButton::clicked, this, &TypeTracker::onStartButtonClicked);
    connect(ui->inputLineEdit, &QLineEdit::textChanged, this, &TypeTracker::onInputTextChanged);
}

TypeTracker::~TypeTracker()
{
    delete ui;
}

void TypeTracker::onStartButtonClicked()
{
    ui->inputLineEdit->setEnabled(true);
    m_timer->stop();
    m_counterTime.setHMS(0, 0, 0);
    ui->counterTimeEdit->setTime(m_counterTime);

    ui->inputLineEdit->clear();
    ui->inputLineEdit->setFocus();

    m_userWords.assign(m_words.size(), QString());
    m_wordIndex = 0;
    m_error = 0;
    m_amount = 0;
    m_chars = 0;

    // Show first word immediately
    if (!m_words.isEmpty()) {
        ui->wordTextEdit->setText(m_words[m_wordIndex]);
    }

    m_timer->start(1000);
}

void TypeTracker::updateCounter()
{
    m_counterTime = m_counterTime.addSecs(1);
    ui->counterTimeEdit->setTime(m_counterTime);

    // When 1 minute passes or all words are done
    if (m_counterTime.minute() >= 1 || m_wordIndex >= m_words.size()) {
        m_timer->stop();

        float wpm = (m_chars / 5.0f) / 1.0f; // per minute
        float accuracy = 0.0f;
        if (m_chars > 0)
            accuracy = ((m_chars - m_amount) / static_cast<float>(m_chars)) * 100.0f;

        QMessageBox::information(this,
                                 "ScoreBoard",
                                 QString("You typed %1 chars.\nWPM: %2\nAccuracy: %3%")
                                     .arg(m_chars)
                                     .arg(wpm)
                                     .arg(accuracy));

        // Reset everything after user closes the message box
        resetSession();
    }
}


void TypeTracker::onInputTextChanged(const QString &text)
{
    if (m_wordIndex >= m_words.size())
        return;

    QString currentWord = m_words[m_wordIndex];

    int minLength = qMin(text.length(), currentWord.length());
    int wordErrors = 0;

    for (int i = 0; i < minLength; ++i) {
        if (text[i] != currentWord[i])
            wordErrors++;
    }

    // If user finishes the word (length matches)
    if (text.length() == currentWord.length()) {
        m_userWords[m_wordIndex] = text;
        m_amount += wordErrors;
        m_chars += text.length();
        m_wordIndex++;

        ui->missCounterLineEdit->setText(QString::number(m_amount));

        // Move to next word
        if (m_wordIndex < m_words.size()) {
            ui->wordTextEdit->setText(m_words[m_wordIndex]);
        } else {
            ui->wordTextEdit->setText("Done!");
        }

        ui->inputLineEdit->clear();
    }
}

void TypeTracker::resetSession()
{
    m_timer->stop();
    m_counterTime.setHMS(0, 0, 0);
    ui->counterTimeEdit->setTime(m_counterTime);

    ui->inputLineEdit->clear();
    ui->wordTextEdit->clear();
    ui->missCounterLineEdit->clear();

    m_wordIndex = 0;
    m_amount = 0;
    m_chars = 0;
    m_userWords.clear();

    // Optionally disable input until "Start" is pressed again
    ui->inputLineEdit->setEnabled(false);
}

