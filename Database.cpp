#include "Database.h"


void Database::PopulateQuestionsStorage()
{
    m_db.sync_schema();

    auto initProductsCount = m_db.count<Question>();

    QuestionManager qm;
    qm.ReadNumericFromFile();
    qm.ReadGridsFromFile();

    m_allQuestions = qm.m_n;
    m_allQuestions.insert(m_allQuestions.end(), qm.m_g.begin(), qm.m_g.end());
    qm.m_g.clear();
    qm.m_n.clear();

    if (initProductsCount == 0)
    {
        m_db.insert_range(m_allQuestions.begin(), m_allQuestions.end());
    }
    else return;
}

int Database::GetUserIdforPlayer(std::string name)
{
    for (int i = 0; i < m_users.size(); i++)
    {
        if (m_users[i].name == name)
        {
            return m_users[i].id;
        }
    }
}

Question Database::GenerateRandomQuestion(char c)
{

    std::random_device rd;
    std::mt19937 eng(rd());
    int id;
    bool ok = false;

    while (!ok)
    {
        if (c == 'N')
        {
            std::uniform_int_distribution<> distr(1, 50);
            id = distr(eng);
        }
        else if (c == 'G')
        {
            std::uniform_int_distribution<> distr(51, 100);
            id = distr(eng);
        }

        auto question = m_db.get_pointer<Question>(id);
        Question q{ question->m_id, question->m_question, question->m_answerOrIndex, question->m_grids };

        if (q.m_question != "")
        {
            ok = true;
            m_allQuestionsRemoved.push_back(q);
            RemoveQuestionFromDb(q);
            return q;
        }
    }

}

void Database::RemoveQuestionFromDb(Question q)
{
    q.m_question = "";
    q.m_answerOrIndex = 0;
    q.m_grids.clear();

    m_db.update(q);
}

void Database::UpdateDataBase()
{
    for (int i = 0; i < m_allQuestionsRemoved.size(); i++)
    {
        m_db.update(m_allQuestionsRemoved[i]);
    }
}

void Database::PopulateStorage()
{
    m_db.sync_schema();
    auto count = m_db.count<User>();
    auto allUsers = m_db.get_all<User>();
    for (int i = 0; i < allUsers.size(); i++)
    {
        m_users.push_back(allUsers[i]);
    }

    if (count != 0)
        return;
    std::vector<User> users = {
        User{ -1, "Andrei",  "parola1",23, 5},
        User{ -1, "Maria",   "parola2", 6, 2},
        User{ -1, "Mike","parola3", 7, 3 },
        User{ -1, "Ionut","parola4", 9, 1 },
        User{ -1, "Finn",  "parola5", 7, 3 },
       User{ -1, "Jake",  "parola6", 26, 12}
    };
    m_db.insert_range(users.begin(), users.end());
    allUsers = m_db.get_all<User>();
    for (int i = 0; i < allUsers.size(); i++)
    {
        m_users.push_back(allUsers[i]);
    }
}
;
void Database::InsertIntoDatabase(const User& user)
{
    m_db.insert(user);
    m_users.push_back(user);
};

bool Database::IsAlreadyTaken(const std::string& username)
{

    for (int i = 0; i < m_users.size(); i++)
    {
        std::cout << m_users[i].name << " ";
        if (m_users[i].name == username)
            return true;
    }
    return false;
}

bool Database::IsPasswordValid(const std::string& name,const std::string& password)
{
    for (int i = 0; i < m_users.size(); i++)
    {
        if (m_users[i].name == name && m_users[i].password == password)
            return true;
    }
    return false;
}

void Database::IncrementGamesPlayed(const std::string& name)
{
    for (int i = 0; i < m_users.size(); i++)
    {
        if (m_users[i].name == name)
        {
            m_users[i].numberOfGamesPlayed++;
            m_db.update(m_users[i]);
        }

    }
}

std::vector<Question>& Database::GetAllQuestionsVector() { return m_allQuestions; }
