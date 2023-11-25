#pragma once

#define SINGLETON(ClassType)                                                                                           \
  public:                                                                                                              \
    static ClassType *GetInst()                                                                                        \
    {                                                                                                                  \
        static ClassType inst;                                                                                         \
        return &inst;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
  public:                                                                                                              \
    ClassType *operator=(const ClassType &_Origin) = delete;                                                           \
                                                                                                                       \
  private:                                                                                                             \
    ClassType();                                                                                                       \
    ClassType(const ClassType &_Origin) = delete;                                                                      \
    ~ClassType();