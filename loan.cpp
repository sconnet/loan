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

void usage()
{
    std::cout << "\nUsage: loan -p principle [-i interest_rate | -t loan_period]"
              << "\n       loan -m payment [-i interest_rate | -t loan_period]"
              << "\nExample: loan -i 7.0 -p 39000.00 -t 60.0\n\n"
              << "-i  simple yearly interest rate\n"
              << "-p  principle amount of loan\n"
              << "-t  loan period in months (ie. number of payments)\n"
              << "-m  monthly payment\n"
              << "-h  help I don't understand\n\n"
              << "ordering of arguments does not matter\n"
              << "unspecified arguments will be solved if possible\n"
              << "Report bugs to <steve.connet@cox.net>\n\n";
}

void help()
{
    std::cout << "Definitions:\n";
    std::cout << "Break Even Years = number of years to pay off principle if payment went to principle alone.\n";
    std::cout << "Interest% = Total interest paid as a percentage of Principal.\n";
}

// ----------------------------------------------------------------------------

// calculate monthly payment given interest and period
void calcPayment(double principleAmount, double yearlyInterestRate, double numberPayments)
{
    double monthlyInterestRate = yearlyInterestRate / 1200.0;
    double x = pow( 1 + monthlyInterestRate, -numberPayments );
    double monthlyPayment = principleAmount * monthlyInterestRate / ( 1 - x );

    double totalPaid = monthlyPayment * numberPayments;
    double interestPaid = totalPaid - principleAmount;
    double interestPaidPercent = (interestPaid / principleAmount) * 100.0;

    double breakEvenYears = ( principleAmount / monthlyPayment ) / 12.0;

    std::cout << std::setw( 12 ) << std::left << std::fixed << std::showpoint << std::setprecision( 2 )
              << "Monthly = " << monthlyPayment
              << "\tPayments = " << numberPayments
              << std::showpoint << std::setprecision( 3 )
              << "\tRate = " << yearlyInterestRate
              << std::showpoint << std::setprecision( 2 )
              << "\tInterest = " << interestPaid
              << "\tTotal = " << totalPaid
              << "\tInterest% = " << interestPaidPercent << "%"
              << "\tBreakevenYrs = " << breakEvenYears
              << std::endl;
}

// calculate monthly payment given interest
void calcPaymentAndPeriod(double principleAmount, double yearlyInterestRate)
{
    double numberPayments = 12;
    while( numberPayments < 361 )
    {
        calcPayment( principleAmount, yearlyInterestRate, numberPayments );
        numberPayments += 12.0;
    }
}

// calculate monthly payment given period
void calcPaymentAndInterest(double principleAmount, double numberPayments)
{
    double interestRate = 1.0;
    while( interestRate < 26.0 )
    {
        calcPayment( principleAmount, interestRate, numberPayments );
        interestRate += 1.0;
    }
}

// calculate payment, period, and interest
void calcPaymentPeriodAndInterest(double principleAmount)
{
    double numberPayments = 12;
    while( numberPayments < 361 )
    {
        calcPaymentAndInterest( principleAmount, numberPayments );
        numberPayments += 12.0;

        std::cout << std::endl;
    }
}

// ----------------------------------------------------------------------------

// calculate principle given period and interest
void calcPrinciple(double monthlyPayment, double numberPayments, double yearlyInterestRate)
{
    double monthlyInterestRate = yearlyInterestRate / 1200.0;
    double x = std::pow( 1 + monthlyInterestRate, -numberPayments );
    double principleAmount =  monthlyPayment * (1 - x) / monthlyInterestRate;

    calcPayment(principleAmount, yearlyInterestRate, numberPayments);
}

// calculate principle and interest given period
void calcPrincipleAndInterest(double monthlyPayment, double numberPayments)
{
    double interestRate = 1.0;
    while( interestRate < 25.0 )
    {
        calcPrinciple( monthlyPayment, numberPayments, interestRate );
        interestRate += 1.0;
    }
}

// calculate principle and period given interest
void calcPrincipleAndPeriod(double monthlyPayment, double yearlyInterestRate)
{
    double numberPayments = 12;
    while( numberPayments < 361 )
    {
        calcPrinciple( monthlyPayment, numberPayments, yearlyInterestRate );
        numberPayments += 12.0;
    }
}

// calculate principle, period, and interest
void calcPrinciplePeriodAndInterest(double monthlyPayment)
{
    double numberPayments = 12;
    while( numberPayments < 361 )
    {
        calcPrincipleAndInterest( monthlyPayment, numberPayments );
        numberPayments += 12.0;

        std::cout << std::endl;
    }
}

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    double principleAmount = -999;
    double monthlyPayment = -999;
    double yearlyInterestRate = -999;
    double numberPayments = -999;
    int retval = EXIT_FAILURE;

    if( --argc == 0 )
    {
        usage();
        return retval;
    }
    else while( argc )
        {
            if( strcmp( argv[argc], "-h" ) == 0 )
            {
                help();
                return retval;
            }
            else if( strcmp( argv[argc], "-i" ) == 0 )
            {
                yearlyInterestRate = strtod( argv[argc + 1], NULL );
            }
            else if( strcmp( argv[argc], "-p" ) == 0 )
            {
                principleAmount = strtod( argv[argc + 1], NULL );
            }
            else if( strcmp( argv[argc], "-t" ) == 0 )
            {
                numberPayments = strtod( argv[argc + 1], NULL );
            }
            else if( strcmp( argv[argc], "-m" ) == 0 )
            {
                monthlyPayment = strtod( argv[argc + 1], NULL );
            }
            --argc;
        }

    // invalid, must have at least principle or payment
    if( principleAmount < 0 && monthlyPayment < 0 )
    {
        usage();
    }
    else if( principleAmount > 0 && monthlyPayment > 0 )
    {
        usage();
        std::cout << "Cannot specify BOTH -m and -p arguments at the same time\n";
    }

    // (-m) solve for principle amount
    else if( monthlyPayment > 0 )
    {
        retval = EXIT_SUCCESS;

        if( numberPayments > 0 && yearlyInterestRate > 0 )
        {
            calcPrinciple( monthlyPayment, numberPayments, yearlyInterestRate );
        }
        else if( yearlyInterestRate > 0 )
        {
            calcPrincipleAndPeriod( monthlyPayment, yearlyInterestRate );
        }
        else if( numberPayments > 0 )
        {
            calcPrincipleAndInterest( monthlyPayment, numberPayments );
        }
        else
        {
            calcPrinciplePeriodAndInterest( monthlyPayment );
        }
    }

    // (-p) solve for monthly payment
    else if( principleAmount > 0 )
    {
        retval = EXIT_SUCCESS;

        if( numberPayments > 0 && yearlyInterestRate > 0 )
        {
            calcPayment( principleAmount, yearlyInterestRate, numberPayments );
        }
        else if( yearlyInterestRate > 0 )
        {
            calcPaymentAndPeriod( principleAmount, yearlyInterestRate );
        }
        else if( numberPayments > 0 )
        {
            calcPaymentAndInterest( principleAmount, numberPayments );
        }
        else
        {
            calcPaymentPeriodAndInterest( principleAmount );
        }
    }
    else
    {
        help();
    }

    return retval;
}

