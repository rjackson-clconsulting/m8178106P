#ifndef _CRRT_TESTS_H_
#define _CRRT_TESTS_H_

/**
  * Struct to activate test and error simulation on the microcontrollers
  * - if the name of the test ends with "_C", this is a test for the controller uC
  * - if the name of the test ends with "_P", this is a test for the protective uC
  * - if the name of the test ends with "_CP", this is a test for the both the uC
  */
struct tests_bitF_t
{
    uint16_t test_001 :1;
    uint16_t test_002 :1;
    uint16_t test_003 :1;
    uint16_t test_004 :1;
    uint16_t test_005 :1;
    uint16_t test_006 :1;
    uint16_t test_007 :1;
    uint16_t test_008 :1;

    uint16_t test_009 :1;
    uint16_t test_010 :1;
    uint16_t test_011 :1;
    uint16_t test_012 :1;
    uint16_t test_013 :1;
    uint16_t test_014 :1;
    uint16_t test_015 :1;
    uint16_t test_016 :1;

    uint16_t test_017 :1;
    uint16_t test_018 :1;
    uint16_t test_019 :1;
    uint16_t test_020 :1;
    uint16_t test_021 :1;
    uint16_t test_022 :1;
    uint16_t test_023 :1;
    uint16_t test_024 :1;

    uint16_t test_025 :1;
    uint16_t test_026 :1;
    uint16_t test_027 :1;
    uint16_t test_028 :1;
    uint16_t test_029 :1;
    uint16_t test_030 :1;
    uint16_t test_031 :1;
    uint16_t test_032 :1;

#ifdef __cplusplus
    tests_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(tests_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    tests_bitF_t(const tests_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(tests_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    tests_bitF_t& operator= (const tests_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(tests_bitF_t); i++)
                *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
	{
		for( uint16_t i = 0; i < sizeof(tests_bitF_t); i++)
			*((uint8_t*)(((uint8_t*)this)+i)) = 0;
	}
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct tests_bitF_t 	tests_bitF_t;
#endif


/**
  * Struct to activate test and error simulation on the microcontrollers
  * - if the name of the test ends with "_C", this is a test for the controller uC
  * - if the name of the test ends with "_P", this is a test for the protective uC
  * - if the name of the test ends with "_CP", this is a test for the both the uC
  */
struct sps_tests_bitF_t
{
    uint16_t test_001 :1;
    uint16_t test_002 :1;
    uint16_t test_003 :1;
    uint16_t test_004 :1;
    uint16_t test_005 :1;
    uint16_t test_006 :1;
    uint16_t test_007 :1;
    uint16_t test_008 :1;

    uint16_t test_009 :1;
    uint16_t test_010 :1;
    uint16_t test_011 :1;
    uint16_t test_012 :1;
    uint16_t test_013 :1;
    uint16_t test_014 :1;
    uint16_t test_015 :1;
    uint16_t test_016 :1;

    uint16_t test_017 :1;
    uint16_t test_018 :1;
    uint16_t test_019 :1;
    uint16_t test_020 :1;
    uint16_t test_021 :1;
    uint16_t test_022 :1;
    uint16_t test_023 :1;
    uint16_t test_024 :1;

    uint16_t test_025 :1;
    uint16_t test_026 :1;
    uint16_t test_027 :1;
    uint16_t test_028 :1;
    uint16_t test_029 :1;
    uint16_t test_030 :1;
    uint16_t test_031 :1;
    uint16_t test_032 :1;

#ifdef __cplusplus
    sps_tests_bitF_t()
    {
        for( uint16_t i = 0; i < sizeof(sps_tests_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }

    sps_tests_bitF_t(const sps_tests_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_tests_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
    }

    sps_tests_bitF_t& operator= (const sps_tests_bitF_t &__val)
    {
        for( uint16_t i = 0; i < sizeof(sps_tests_bitF_t); i++)
                *((uint8_t*)(((uint8_t*)this)+i)) = *((uint8_t*)(((uint8_t*)&__val)+i));
        return *this;
    }

    void reset()
    {
        for( uint16_t i = 0; i < sizeof(sps_tests_bitF_t); i++)
            *((uint8_t*)(((uint8_t*)this)+i)) = 0;
    }
#endif
}__attribute__ ((packed));
#ifdef __cplusplus
#else
typedef struct sps_tests_bitF_t 	sps_tests_bitF_t;
#endif

#endif // CRRT_TESTS_H
