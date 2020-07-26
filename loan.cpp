/*
   loan
   Steve Connet

   Determines your monthly payment of a simple loan.

   compile with:
   g++ -O2 -o loan loan.cpp -lm

   The following functions are supported:

   1. calculate payment given interest and period
   2. calculate payment given interest
   3. calculate payment given period
   4. calculate payment, period, and interest

   5. calculate principle given period and interest
   6. calculate principle and interest given period
   7. calculate principle and period given interest
   8. calculate principle, period, and interest
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>

#include <unistd.h> // getopt

#define SHOW_DEFAULT 0x00
#define SHOW_PERIOD  0x01
#define SHOW_RATE    0x02

void usage()
{
    std::cout << "\n"
              << "Usage: loan -p principle [-i interest_rate | -t loan_period]"
              << "\n       loan -m payment [-i interest_rate | -t loan_period]"
              << "\nExample: loan -i 7.0 -p 39000.00 -t 60.0\n\n"
              << "-i  simple yearly interest rate\n"
              << "-p  principle amount of loan\n"
              << "-t  loan period in months (ie. number of payments)\n"
              << "-m  monthly payment\n"
              << "-h  help I don't understand\n\n"
              << "Ordering of arguments does not matter.\n"
              << "Unspecified arguments will be solved if possible.\n"
              << "Report bugs to <steve.connet@cox.net>\n"
              << std::endl;
}

void help()
{
    std::cout << "Definitions:\n"
              << "Break Even Years = number of years to pay off principle if"
              << " payment went to principle alone.\n"
              << "Interest% = Total interest paid as a percentage of Principal."
              << std::endl;
}

// ----------------------------------------------------------------------------

// calculate monthly payment given interest and period
void calcPayment(double principleAmount, double yearlyInterestRate,
                 double numberPayments, int options)
{
    double monthlyInterestRate = yearlyInterestRate / 1200.0;
    double x = pow(1 + monthlyInterestRate, -numberPayments);
    double monthlyPayment = principleAmount * monthlyInterestRate / (1 - x);

    double totalPaid = monthlyPayment * numberPayments;
    double interestPaid = totalPaid - principleAmount;
    double interestPaidPercent = (interestPaid / principleAmount) * 100.0;

    double breakEvenYears = (principleAmount / monthlyPayment) / 12.0;

    std::cout << "Monthly: "
              << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << monthlyPayment;

    if(options & SHOW_PERIOD)
    {
        std::cout << "\tNum Payments: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << numberPayments;
    }

    if(options & SHOW_RATE)
    {
        std::cout << "\tRate: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << std::showpoint << std::setprecision(3)
                  << yearlyInterestRate;
    }

    std::cout << "\tInterest: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << interestPaid;

    std::cout << "\tTotal: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << totalPaid;

    std::cout << "\tInterest%: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << interestPaidPercent;

    std::cout << "\tBreakeven: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << breakEvenYears;

    std::cout << std::endl;
}

// calculate monthly payment given interest
void calcPaymentAndPeriod(double principleAmount, double yearlyInterestRate)
{
    double numberPayments = 12;
    while(numberPayments < 361)
    {
        calcPayment(principleAmount, yearlyInterestRate, numberPayments,
                    SHOW_PERIOD);
        numberPayments += 12.0;
    }
}

// calculate monthly payment given period
void calcPaymentAndInterest(double principleAmount, double numberPayments)
{
    double interestRate = 1.0;
    while(interestRate < 26.0)
    {
        calcPayment(principleAmount, interestRate, numberPayments,
                    SHOW_RATE);
        interestRate += 1.0;
    }
}

// calculate payment, period, and interest
void calcPaymentPeriodAndInterest(double principleAmount)
{
    double numberPayments = 12;
    while(numberPayments < 361)
    {
        std::cout << "Num Payments: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << std::showpoint << std::setprecision(3)
                  << numberPayments;
        calcPaymentAndInterest(principleAmount, numberPayments);
        numberPayments += 12.0;

        std::cout << std::endl;
    }
}

// ----------------------------------------------------------------------------

// calculate principle given period and interest
void calcPrinciple(double monthlyPayment, double numberPayments,
                   double yearlyInterestRate, int options)
{
    double monthlyInterestRate = yearlyInterestRate / 1200.0;
    double x = std::pow(1 + monthlyInterestRate, -numberPayments);
    double principleAmount =  monthlyPayment * (1 - x) / monthlyInterestRate;

    double totalPaid = monthlyPayment * numberPayments;
    double interestPaid = totalPaid - principleAmount;
    double interestPaidPercent = (interestPaid / principleAmount) * 100.0;

    double breakEvenYears = (principleAmount / monthlyPayment) / 12.0;

    std::cout << "Principle: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << principleAmount;

    if(options & SHOW_PERIOD)
    {
        std::cout << "\tNum Payments: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << numberPayments;
    }

    if(options & SHOW_RATE)
    {
        std::cout << "\tRate: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << std::showpoint << std::setprecision(3)
                  << yearlyInterestRate;
    }

    std::cout << "\tInterest: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << interestPaid;

    std::cout << "\tTotal: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << totalPaid;

    std::cout << "Interest%: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << interestPaidPercent;

    std::cout << "\tBreakeven: ";
    std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
              << std::setprecision(2)
              << breakEvenYears;

    std::cout << std::endl;
}

// calculate principle and interest given period
void calcPrincipleAndInterest(double monthlyPayment, double numberPayments)
{
    double interestRate = 1.0;
    while(interestRate < 25.0)
    {
        calcPrinciple(monthlyPayment, numberPayments, interestRate,
                      SHOW_RATE);
        interestRate += 1.0;
    }
}

// calculate principle and period given interest
void calcPrincipleAndPeriod(double monthlyPayment, double yearlyInterestRate)
{
    double numberPayments = 12;
    while(numberPayments < 361)
    {
        calcPrinciple(monthlyPayment, numberPayments, yearlyInterestRate,
                      SHOW_PERIOD);
        numberPayments += 12.0;
    }
}

// calculate principle, period, and interest
void calcPrinciplePeriodAndInterest(double monthlyPayment)
{
    double numberPayments = 12;
    while(numberPayments < 361)
    {
        std::cout << "Num Payments: ";
        std::cout << std::setw(12) << std::left << std::fixed << std::showpoint
                  << std::setprecision(2)
                  << std::showpoint << std::setprecision(3)
                  << numberPayments;
        calcPrincipleAndInterest(monthlyPayment, numberPayments);
        numberPayments += 12.0;

        std::cout << std::endl;
    }
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    double principleAmount = -1;
    double monthlyPayment = -1;
    double yearlyInterestRate = -1;
    double numberPayments = -1;
    int retval = EXIT_FAILURE;

    int c;
    while((c = getopt(argc, argv, "h:i:p:t:m:")) != -1)
    {
        switch(c)
        {
            case 'h':
                help();
                break;
            case 'i':
                yearlyInterestRate = strtod(optarg, NULL);
                break;
            case 'p':
                principleAmount = strtod(optarg, NULL);
                break;
            case 't':
                numberPayments = strtod(optarg, NULL);
                break;
            case 'm':
                monthlyPayment = strtod(optarg, NULL);
                break;
            default:
                usage();
                break;
        }
    }

    // invalid, must have at least principle (-p) or monthly payment (-m)
    if(principleAmount < 0 && monthlyPayment < 0)
    {
        usage();
    }
    else if(principleAmount > 0 && monthlyPayment > 0)
    {
        usage();
        std::cout << "Cannot specify BOTH -m and -p arguments at the same time"
                  << std::endl;
    }

    // (-m) solve for principle amount
    else if(monthlyPayment > 0)
    {
        retval = EXIT_SUCCESS;

        if(numberPayments > 0 && yearlyInterestRate > 0)
        {
            calcPrinciple(monthlyPayment, numberPayments, yearlyInterestRate,
                          SHOW_DEFAULT);
        }
        else if(yearlyInterestRate > 0)
        {
            calcPrincipleAndPeriod(monthlyPayment, yearlyInterestRate);
        }
        else if(numberPayments > 0)
        {
            calcPrincipleAndInterest(monthlyPayment, numberPayments);
        }
        else
        {
            calcPrinciplePeriodAndInterest(monthlyPayment);
        }
    }

    // (-p) solve for monthly payment
    else if(principleAmount > 0)
    {
        retval = EXIT_SUCCESS;

        if(numberPayments > 0 && yearlyInterestRate > 0)
        {
            calcPayment(principleAmount, yearlyInterestRate, numberPayments,
                        SHOW_DEFAULT);
        }
        else if(yearlyInterestRate > 0)
        {
            calcPaymentAndPeriod(principleAmount, yearlyInterestRate);
        }
        else if(numberPayments > 0)
        {
            calcPaymentAndInterest(principleAmount, numberPayments);
        }
        else
        {
            calcPaymentPeriodAndInterest(principleAmount);
        }
    }
    else
    {
        usage();
    }

    return retval;
}


// ----------------------------------------------------------------------------
// CODE GRAVEYARD
// ----------------------------------------------------------------------------
#if 0

// so this works and was written by me. But it's pretty ugle so I'm going to
// leave this here for reference and use getopt instead
if(0 == --argc)
{
    usage();
    return retval;
}
else
{
    while(argc)
    {
        char *next = NULL;

        if(strcmp(argv[argc], "-h") == 0)
        {
            help();
            return retval;
        }
        else if(strcmp(argv[argc], "-i") == 0)
        {
            next = argv[argc + 1];
            if(NULL == next || next[0] == '-')
            {
                std::cout << "Interest rate value not specified. "
                          << "Cannot continue."
                          << std::endl;
                return retval;
            }
            else
            {
                yearlyInterestRate = strtod(argv[argc + 1], NULL);
            }
        }
        else if(strcmp(argv[argc], "-p") == 0)
        {
            next = argv[argc + 1];
            if(NULL == next || next[0] == '-')
            {
                std::cout << "Principle value not specified. "
                          << "Cannot continue."
                          << std::endl;
                return retval;
            }
            else
            {
                principleAmount = strtod(argv[argc + 1], NULL);
            }
        }
        else if(strcmp(argv[argc], "-t") == 0)
        {
            next = argv[argc + 1];
            if(NULL == next || next[0] == '-')
            {
                std::cout << "Period value not specified. "
                          << "Cannot continue."
                          << std::endl;
                return retval;
            }
            else
            {
                numberPayments = strtod(argv[argc + 1], NULL);
            }
        }
        else if(strcmp(argv[argc], "-m") == 0)
        {
            next = argv[argc + 1];
            if(NULL == next || next[0] == '-')
            {
                std::cout << "Monthly Payment value not specified. "
                          << "Cannot continue."
                          << std::endl;
                return retval;
            }
            else
            {
                monthlyPayment = strtod(argv[argc + 1], NULL);
            }
        }

        --argc;
    }
}
#endif
