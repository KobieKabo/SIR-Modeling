// Jakob Long
// Jrl4725

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<sstream>
#include<random>

using std::string;
using std::vector;
using std::rand;

// enum of different possible states
enum class Status {vaccinated, recovered, susceptible , sick};

class Person
{
    private:
        // creates class of the status, using the enum class Status  
        Status state;
        int days_sick;
    public:

        // Creates the person object
        Person()
        {
            state = Status::susceptible;
            days_sick = 0;
            // ensures the random num sequence is always different
            srand(time(NULL));
        }

        // Updates the # of sick days & persons condition
        void update()
        {
            if (state == Status::sick)
            {
                days_sick--;
                if(days_sick == 0)
                {
                    state = Status::recovered;
                }
            }
        }

        // infects the person with the given integer, and updates the days_sick var
        void infect(int num_sick_days)
        {
            if (state == Status::susceptible)
            {
                days_sick = num_sick_days;
                state = Status::sick;
            }
        }

        // sets the persons state as vaccinated
        void vaccinate()
        {
            state = Status::vaccinated;
        }

        // returns true, if the person has recovered from illness & is now stable
        bool is_stable()
        {
            return state == Status::recovered;
        }

        // gets the person objects current health state
        Status person_state()
        {
            return state;
        }

        // prints out the status of the person 
        string status_string() {
            std::stringstream s;
            switch(state) {
                case Status::sick:
                    s << days_sick;
                    return "sick (" + s.str() + " to go)";
                    break;
                case Status::susceptible:
                    return "susceptible";
                    break;
                case Status::recovered:
                    return "recovered";
                    break;
                case Status::vaccinated:
                    return "vaccinated";
                    break;
                default:
                    return "Oops, something went wrong.";
                    break;
            }
        }
};



class Population
{
    private:
        int day;
        int size;
        
        int infected_people;
        int susceptible_people;
        int recovered_people;
        int vaccinated_people;

        double probability;
        int contacts;
        int days_sick;

        vector<Person*> peoples;

        public:

        // Creates the population object, taking in a pop count
        Population(int people)
        {
            size = people;
            day = 0;

            infected_people = 0;
            susceptible_people = people;
            recovered_people = 0;
            vaccinated_people = 0;

            probability = 0.0;
            contacts = 0;
            
            days_sick = 0;

            peoples.reserve(people);
            for(int i = 0; i < people; i++)
            {
                peoples.push_back(new Person());
            }
            // ensures the random num sequence is always different
            srand(time(NULL));
        }
        
        // Infects a random person, setting the patient zero.
        void random_infect()
        {
            infected_people = 1;
            int index = (int) rand() % size;
            Person* infected_victim = peoples[index];
            infected_person(infected_victim);
        }

        // infects the person input
        void infected_person(Person* person)
        {
            person->infect(days_sick);
        }

        /*
            Updates the entire population & their states.
            Selects who will be infected, & infects.
            Allows for infection to spread as well.
        */
        void update()
        {
            int new_susceptible_people = 0;
            int new_infected_people = 0;
            int new_recovered_people = 0;
            int new_vax_people = 0;

            vector<Person*> new_infections;

            for(int i =0; i < peoples.size(); i++)
            {
                Person* person = peoples.at(i);
                switch(person->person_state())
                {
                    case Status::susceptible:
                        new_susceptible_people;
                        break;
                    case Status::recovered:
                        new_recovered_people++;
                        break;
                    case Status::vaccinated:
                        new_vax_people++;
                        break;
                    case Status::sick:
                        new_infected_people++;
                        interaction(person,new_infections);
                        break;
                    default:
                        break;
                }
                //updates at the end of the day
                person->update();
            }
            day++;

            for (Person* person : new_infections)
            {
                infected_person(person);
            }
        }

        /* 
            Similar to update function, but only updates the population 
            counts of each possible state a person can have
        */
        void pop_status_update()
        {
            int new_susceptible_people = 0;
            int new_infected_people = 0;
            int new_recovered_people = 0;
            int new_vax_people = 0;

            for(int i = 0; i <peoples.size(); i++)
            {
                Person* person = peoples[i];
                if (person->person_state() == Status::susceptible)
                {
                    new_susceptible_people++;
                    
                }
                else if (person->person_state() == Status::recovered)
                {
                    new_recovered_people++;
                }
                else if (person->person_state() == Status::vaccinated)
                {
                    new_vax_people++;
                }
                else
                    new_infected_people++;
            }
            infected_people = new_infected_people;
            susceptible_people = new_susceptible_people;
            recovered_people = new_recovered_people;
            vaccinated_people = new_vax_people;
        }

        
        // Returns a string to visualize current population states
        string pop_status()
        {   
            string result = "";

            for(int i = 0; i <peoples.size(); i++)
            {
                Person* person = peoples[i];
                if (person->person_state() == Status::susceptible)
                {
                    result.append(" ?");
                }
                else if (person->person_state() == Status::recovered)
                {
                    result.append(" -");
                }
                else if (person->person_state() == Status::vaccinated)
                {
                    result.append(" !");
                }
                else
                    result.append(" +");
            }
            return result;
        }

        // grabs private days_sick var & sets it to the input days_sick val
        void num_days_sick(int days_sick)
        {
            this->days_sick = days_sick;
        }

        // returns the number of people infected  
        int count_infected()
        {
            return infected_people;
        }

        // returns number of recovered people
        int count_recovered()
        {
            return recovered_people;
        }

        // returns number of vaccinated people
        int count_vaxed()
        {
            return vaccinated_people;
        }

        // returns # of susceptible_people
        int count_susceptible()
        {
            return susceptible_people;
        }
        

        // grabs private prob var & sets it to the input prob val
        void probability_of_transfer(double probability)
        {
            this->probability = probability;
        }

        /*
            Simulates the interaction between people
            Bases infection chance off of probability.
        */
        void interaction(Person* person, vector<Person*> &new_infections)
        {
            for (int i = 0; i < contacts; i++)
            {
                double bad_luck = (double) rand() / (double) RAND_MAX;
                if(bad_luck <= probability)
                {
                    int randnum = rand()%peoples.size();
                    new_infections.push_back(peoples.at(randnum));
                }
            }
        }
    
        /*
         vaccinates a number of the population randomly, 
         total # of vaccinations comes from the set_vax function
        */
        void vaccinate()
        {
            if(susceptible_people < vaccinated_people)
            {
                vaccinated_people = susceptible_people;
            }
            for (int i = 0; i < vaccinated_people; i++)
            {
                int rand_person = rand()%peoples.size();
                Person* cured  = peoples[rand_person];
                if(cured->person_state() == Status::susceptible)
                {
                    cured->vaccinate();
                }
                else
                    i--;
            }
            susceptible_people -= vaccinated_people;
        }

        // grabs the privacte vaccinated_people var & sets it to the vaccinated input val
        void set_vaxxed(int vaccinated)
        {
            this->vaccinated_people = vaccinated;
        }

        // grabs the private contacts var & sets it to the input contacts val
        void set_contact_count(int contacts)
        {
            this->contacts = contacts;
        }

};