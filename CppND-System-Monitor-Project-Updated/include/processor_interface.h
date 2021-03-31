#ifndef PROCESSOR_INTERFACE_H
#define PROCESSOR_INTERFACE_H


class ProcessorInterface {
  public:
    virtual ~ProcessorInterface() {};
    virtual float Utilization() = 0;
};

#endif
