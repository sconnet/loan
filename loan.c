/*
   loan
   Steve Connet

   Determines your monthly payment of a simple loan.

   compile with:
   cc -O2 -o loan loan.c -lm

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

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SHOW_DEFAULT   0x00
#define SHOW_PAYMENT   0x01
#define SHOW_PERIOD    0x02
#define SHOW_RATE      0x04
#define SHOW_PRINCIPLE 0x08

void usage()
{
    printf("\nUsage: loan -p principle [-i interest_rate | -t loan_period]"
            "\n       loan -m payment [-i interest_rate | -t loan_period]"
            "\nExample: loan -i 7.0 -p 39000.00 -t 60.0\n\n"
            "-i  simple yearly interest rate\n"
            "-p  principle amount of loan\n"
            "-t  loan period in months (ie. number of payments)\n"
            "-m  monthly payment\n"
            "-h  help I don't understand\n\n"
            "ordering of arguments does not matter\n"
            "unspecified arguments will be solved if possible\n"
            "Report bugs to <steve.connet@cox.net>\n\n");
}

void help()
{
    printf("Definitions:\n"
            "Break Even Years = number of years to pay off principle if"
            " payment went to principle alone.\n"
            "Interest%% = Total interest paid as a percentage of Principal.\n");
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

    printf("Monthly: %-12.2f\t", monthlyPayment);

    if (options & SHOW_PERIOD)
    {
        printf("Num Payments: %-12.2f\t", numberPayments);
    }

    if (options & SHOW_RATE)
    {
        printf("Rate: %-12.2f\t", yearlyInterestRate);
    }

    printf("Interest: %-12.2f\tTotal: %-12.2f\tInterest%%: %-12.2f"
           "\tBreakeven: %-12.2f\n",
           interestPaid, totalPaid, interestPaidPercent, breakEvenYears);
}

// calculate monthly payment given interest
void calcPaymentAndPeriod(double principleAmount, double yearlyInterestRate)
{
    double numberPayments = 12;
    while (numberPayments <= 360)
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
    while (interestRate < 26.0)
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
    while (numberPayments <= 360)
    {
        printf("Num Payments: %-12.2f\n", numberPayments);
        calcPaymentAndInterest(principleAmount, numberPayments);
        numberPayments += 12.0;

        printf("\n");
    }
}

// ----------------------------------------------------------------------------

// calculate principle given period and interest
void calcPrinciple(double monthlyPayment, double numberPayments,
        double yearlyInterestRate, int options)
{
    double monthlyInterestRate = yearlyInterestRate / 1200.0;
    double x = pow(1 + monthlyInterestRate, -numberPayments);
    double principleAmount =  monthlyPayment * (1 - x) / monthlyInterestRate;

    double totalPaid = monthlyPayment * numberPayments;
    double interestPaid = totalPaid - principleAmount;

    double breakEvenYears = (principleAmount / monthlyPayment) / 12.0;

    printf("Principle: %-12.2f\t", principleAmount);

    if (options & SHOW_PERIOD)
    {
        printf("Payments: %-12.2f\t", numberPayments);
    }

    if (options & SHOW_RATE)
    {
        printf("Rate: %-12.2f\t", yearlyInterestRate);
    }

    printf("Interest: %-12.2f\tTotal: %-12.2f\tBreakeven: %-12.2f\n",
            interestPaid, totalPaid, breakEvenYears);
}

// calculate principle and interest given period
void calcPrincipleAndInterest(double monthlyPayment, double numberPayments)
{
    double interestRate = 1.0;
    while (interestRate < 25.0)
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
    while (numberPayments <= 360)
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
    while (numberPayments <= 360)
    {
        printf("Num Payments: %-12.2f\n", numberPayments);
        calcPrincipleAndInterest(monthlyPayment, numberPayments);
        numberPayments += 12.0;

        printf("\n");
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

    if(0 == --argc)
    {
        usage();
        return retval;
    }

    else while(argc)
    {
        //        printf("argc = %d, argv = %s\n", argc, argv[argc]);
        if(strcmp(argv[argc], "-h") == 0)
        {
            help();
        }
        else if(strcmp(argv[argc], "-i") == 0)
        {
            yearlyInterestRate = strtod(argv[argc + 1], NULL);
        }
        else if(strcmp(argv[argc], "-p") == 0)
        {
            principleAmount = strtod(argv[argc + 1], NULL);
        }
        else if(strcmp(argv[argc], "-t") == 0)
        {
            numberPayments = strtod(argv[argc + 1], NULL);
        }
        else if(strcmp(argv[argc], "-m") == 0)
        {
            monthlyPayment = strtod(argv[argc + 1], NULL);
        }

        --argc;
    }

    //    printf("yearlyInterestRate: %-12.2f\n", yearlyInterestRate);
    //    printf("principleAmount: %-12.2f\n", principleAmount);
    //    printf("numberPayments: %-12.2f\n", numberPayments);
    //    printf("monthlyPayment: %-12.2f\n", monthlyPayment);

    // invalid, must have at least principle or payment
    if(principleAmount < 0 && monthlyPayment < 0)
    {
        usage();
    }
    else if(principleAmount > 0 && monthlyPayment > 0)
    {
        usage();
        printf("Cannot specify BOTH -m and -p arguments at the same time\n");
    }

    // (-m) solve for principle amount
    else if(monthlyPayment > 0)
    {
        retval = EXIT_SUCCESS;

        if(numberPayments > 0 && yearlyInterestRate > 0)
        {
            // calculate principle given period and interest
            calcPrinciple(monthlyPayment, numberPayments, yearlyInterestRate,
                    SHOW_DEFAULT);
        }
        else if(yearlyInterestRate > 0)
        {
            // calculate principle and period given interest
            calcPrincipleAndPeriod(monthlyPayment, yearlyInterestRate);
        }
        else if(numberPayments > 0)
        {
            // calculate principle and interest given period
            calcPrincipleAndInterest(monthlyPayment, numberPayments);
        }
        else
        {
            // calculate principle, period, and interest
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
        help();
    }

    return retval;
}
