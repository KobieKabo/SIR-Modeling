// Jakob Long
// Jrl4725

#include "infect_lib.h"

int main()
{
    int day = 1;
    int large_pop = 30000;
    Population pop(large_pop);
    pop.set_vaxxed(large_pop*0.4);
    pop.vaccinate();
    pop.set_contact_count(6);
    pop.num_days_sick(5);
    // loops for the desired number of days.
    for (day; day <= 100; day++)
    {
        pop.probability_of_transfer(0.3);
        pop.update();
        double bad_luck = (double) rand() / (double) RAND_MAX;
        if (bad_luck > .95)
        {     
            pop.random_infect();
        }
        pop.pop_status_update();
        
        std::cout << "Day # "<< day << ": #sick: " << pop.count_infected() << " |"
                  << " #rcvrd: " << pop.count_recovered() << " |" << " #vax: "
                  << pop.count_vaxed() << " |" << " # susceptible: " <<pop.count_susceptible() << std::endl;
    }
    return 0;
}