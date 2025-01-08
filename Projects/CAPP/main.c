int time_ms = 0;
void delay(int delay)
{
    time_ms += delay;

}

void set_pwm(float* duty_cycle)
{
    printf("time=%d duty_cycle=%f", time_ms, *duty_cycle); // Testing
}

// Your code bellow

// 1 Cycle is 0-100-0.
int cycles_count = 0;

void pwm_handler()
{

    static float increment = 0.1f;
    static float duty_cycle = 0.0f;
    
    // set_pwm()
    // increment
    // Check if out of bounds
   

    set_pwm(&duty_cycle);


    duty_cycle += increment;
    if (duty_cycle  > 100){
        increment *= -1;
       duty_cycle = 100;
        return;

    }

    if (duty_cycle  < 0){
        increment *=-1;
        cycles_count++;
        duty_cycle = 0;
        return;
    }
    
        

}

// Every 2 seconds, the duty cycle should go from 0%-100%-0%
void main()
{
    while(1)
    {
        pwm_handler();
        delay(1); // 1ms
    }
}