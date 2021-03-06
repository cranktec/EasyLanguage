#include "trainer.h"

void Trainer::increaseLearningState(Voc *p_pvocCurrent)
{
    if(static_cast<int>(p_pvocCurrent->getLearningState()) > static_cast<int>(Global::PERFECT))
    {
       p_pvocCurrent->setLearningState(static_cast<Global::LearningState>(static_cast<int>(p_pvocCurrent->getLearningState()) - 1));
    }
}

void Trainer::decreaseLearningState(Voc *p_pvocCurrent)
{
    if(static_cast<int>(p_pvocCurrent->getLearningState()) < static_cast<int>(Global::BAD))
    {
        p_pvocCurrent->setLearningState(static_cast<Global::LearningState>(static_cast<int>(p_pvocCurrent->getLearningState()) + 1));
    }
}

Trainer::Trainer()
{
}

Trainer::~Trainer()
{

}

void Trainer::startTraining(Language *p_plangValue, int p_iSizeOfVocsToLearn)
{
    m_stCurrentIndex = 0;
    std::vector<std::tuple<Voc*, bool>> mppvocAllVocs;
    for(size_t stLectionIndex = 0; stLectionIndex < p_plangValue->getLectionListSize(); stLectionIndex++)
    {
        for(size_t stVocIndex = 0; stVocIndex < p_plangValue->getLection(stLectionIndex)->getVocListSize(); stVocIndex++)
        {
            mppvocAllVocs.push_back(std::make_tuple(p_plangValue->getLection(stLectionIndex)->getVoc(stVocIndex), false));
        }
    }

    for(int iCounter = 0; iCounter < p_iSizeOfVocsToLearn; iCounter++)
    {
        size_t stRandomIndex = qrand() % (mppvocAllVocs.size());
        if(std::get<1>(mppvocAllVocs.at(stRandomIndex)) == false)
        {
            m_mppvocVocToTrain.push_back(std::get<0>(mppvocAllVocs.at(stRandomIndex)));
            std::get<1>(mppvocAllVocs.at(stRandomIndex)) = true;
        }
    }
}

void Trainer::startTraining(Lection *p_plectLectionToTrain, int p_iSizeOfVocsToLearn)
{
    m_stCurrentIndex = 0;
    std::vector<std::tuple<Voc*, bool>> mppvocAllVocs;
    for(size_t stVocIndex = 0; stVocIndex < p_plectLectionToTrain->getVocListSize(); stVocIndex++)
    {
        mppvocAllVocs.push_back(std::make_tuple(p_plectLectionToTrain->getVoc(stVocIndex), false));
    }

    for(int iCounter = 0; iCounter < p_iSizeOfVocsToLearn; iCounter++)
    {
        size_t stRandomIndex = qrand() % (mppvocAllVocs.size());
        if(std::get<1>(mppvocAllVocs.at(stRandomIndex)) == false)
        {
            m_mppvocVocToTrain.push_back(std::get<0>(mppvocAllVocs.at(stRandomIndex)));
            std::get<1>(mppvocAllVocs.at(stRandomIndex)) = true;
        }
    }
}

bool Trainer::isCorrect(Voc *p_pvocCurrent, QString p_qstTry)
{
    if(p_pvocCurrent->getVocType() == Global::VERB)
    {
        Verb *pverbCurrent = dynamic_cast<Verb*>(p_pvocCurrent);
        if(pverbCurrent->getOnlyVerb() == p_qstTry)
        {
            increaseLearningState(p_pvocCurrent);
            return true;
        }
    }
    else
    {
        if(p_pvocCurrent->getWord() == p_qstTry)
        {
            increaseLearningState(p_pvocCurrent);
            return true;
        }
    }
    decreaseLearningState(p_pvocCurrent);
    return false;
}

bool Trainer::hasNext()
{
    if(m_stCurrentIndex < m_mppvocVocToTrain.size() && m_stCurrentIndex >= 0)
    {
        return true;
    }
    return false;
}

Voc *Trainer::next()
{
    try
    {
        m_pvocCurrentVoc = m_mppvocVocToTrain.at(m_stCurrentIndex++);
        return m_pvocCurrentVoc;
    }
    catch(std::exception &e)
    {
        throw e;
    }
}

QString Trainer::getRandomDefintion(Voc *p_pvocCurrent)
{
    try
    {
        size_t stRandomIndex = qrand() % (p_pvocCurrent->getDefinitionListSize());
        return p_pvocCurrent->getDefinition(stRandomIndex);
    }
    catch(std::exception &e) { throw e;}
}

void Trainer::stopTraining()
{
    m_mppvocVocToTrain.clear();
    m_stCurrentIndex = -1;
}

Voc *Trainer::getCurrentVoc() const
{
    return m_pvocCurrentVoc;
}
