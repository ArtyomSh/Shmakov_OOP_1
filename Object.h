#include <utility>
#include <cstring>
#include <climits>
#include <cstdlib>

#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
    Object()
    {
        objects_count++;
    }
    ~Object()
    {
        objects_count--;
    }

    virtual std::string toString() const = 0;

    static int get_objects_count()
    {
        return Object::objects_count;
    }

private:
    static inline int objects_count;
};

class Named : public Object
{
public:
    explicit Named(std::string data)
    {
        name = std::move(data);
    }
    std::string name;
};

class Task : public Object
{
public:
    Task()
    {
        completed = false;
    };
    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual bool hasOutput() const = 0;
    bool isCompleted() const
    {
        return completed;
    }

protected:
    bool completed;
};

class BinaryTask : public Task, public Named
{
public:
    BinaryTask() : Task(), Named("Binary") {}
    bool hasOutput() const override
    {
        return true;
    }
    double getOutput() const
    {
        return output;
    }

protected:
    double op1 = 0.0;
    double op2 = 0.0;
    double output = 0.0;
};

class AdditionTask : public BinaryTask
{
public:
    AdditionTask() : BinaryTask() {}
    AdditionTask(double first, double second)
    {
        output = 0;
        op1 = first;
        op2 = second;
    }
    void execute() override
    {
        output = op1 + op2;
        completed = true;
    }
    std::string toString() const override
    {
        if (isCompleted())
        {
            return name + " task: " + std::to_string(op1) + " + " + std::to_string(op2) + " = " + std::to_string(output);
        }
        else
            return name + " task: " + "need to add " + std::to_string(op2) + " and " + std::to_string(op1);
    }
};

class MultiplicationTask : public BinaryTask
{
public:
    MultiplicationTask() : BinaryTask() {}
    MultiplicationTask(double first, double second)
    {
        output = 0;
        op1 = first;
        op2 = second;
    }
    void execute() override
    {
        output = op1 * op2;
        completed = true;
    }
    std::string toString() const override
    {
        if (isCompleted())
        {
            return name + " task: " + std::to_string(op1) + " * " + std::to_string(op2) + " = " + std::to_string(output);
        }
        else
            return name + " task: " + "need to multiply " + std::to_string(op1) + " by " + std::to_string(op2);
    }
};

class SubtractionTask : public BinaryTask
{
public:
    SubtractionTask() : BinaryTask() {}
    SubtractionTask(double first, double second)
    {
        output = 0;
        op1 = first;
        op2 = second;
    }
    void execute() override
    {
        output = op1 - op2;
        completed = true;
    }
    std::string toString() const override
    {
        if (isCompleted())
        {
            return name + " task: " + std::to_string(op1) + " - " + std::to_string(op2) + " = " + std::to_string(output);
        }
        else
            return name + " task: " + "need to subtract " + std::to_string(op2) + " from " + std::to_string(op1);
    }
};

class DivisionTask : public BinaryTask
{
public:
    DivisionTask() : BinaryTask() {}
    DivisionTask(double first, double second)
    {
        output = 0;
        op1 = first;
        op2 = second;
    }
    void execute() override
    {
        if (std::abs(op2) - 1e-5 > 0)
        {
            output = op1 / op2;
            completed = true;
        }
        else
            throw std::runtime_error("Division by zero");
    }
    std::string toString() const override
    {
        if (isCompleted())
        {
            return name + " task: " + std::to_string(op1) + " / " + std::to_string(op2) + " = " + std::to_string(output);
        }
        else
        {
            return name + " task: " + "need to divide " + std::to_string(op1) + " by " + std::to_string(op2);
        }
    }
};

class CountTasksInContainer : public Task
{
public:
    CountTasksInContainer(Container<Task> *container)
    {
        output = 0;
        task_container = container;
    }
    void execute() override
    {
        if (!completed)
        {
            output = task_container->get_size();
            completed = true;
        }
        else
            throw std::runtime_error("Task already done");
    }
    bool hasOutput() const override
    {
        return true;
    };
    std::string toString() const override
    {
        if (isCompleted())
        {
            return "There are " + std::to_string(output) + " objects in the container";
        }
        else
        {
            return "This task counts the number of objects in the container";
        }
    }

private:
    uint32_t output;
    Container<Task> *task_container;
};

class TaskAddAnotherTask : public Task
{
public:
    TaskAddAnotherTask(Task *task, Container<Task> *container)
    {
        task_container = container;
        new_task = task;
    }
    void execute() override
    {
        if (!completed)
        {
            task_container->push_back(new_task);
            completed = true;
        }
        else
            throw std::runtime_error("Task already executed");
    };
    bool hasOutput() const override
    {
        return false;
    };
    std::string toString() const override
    {
        if (isCompleted())
        {
            return "Task added";
        }
        else
        {
            return "This task adds new task";
        }
    }

private:
    Task *new_task;
    Container<Task> *task_container;
};

class CountResultTasksInContainer : public Task
{
public:
    CountResultTasksInContainer(Container<Task> *container)
    {
        output = 0;
        task_container = container;
    }
    void execute() override
    {
        if (!completed)
        {
            for (auto x : *task_container)
            {
                output += x->hasOutput();
            }
            completed = true;
        }
        else
            throw std::runtime_error("Task already done");
    }
    bool hasOutput() const override
    {
        return true;
    };
    std::string toString() const override
    {
        if (isCompleted())
        {
            return "There are " + std::to_string(output) + " results task in the container";
        }
        else
        {
            return "This task counts the number of results task in the container";
        }
    }

private:
    int output;
    Container<Task> *task_container;
};

class TaskClearContainerTask : public Task
{
public:
    TaskClearContainerTask(Container<Task> *container)
    {
        task_container = container;
    }
    void execute() override
    {
        if (!completed)
        {
            task_container->clear();
            completed = true;
        }
        else
            throw std::runtime_error("Task already done");
    }
    bool hasOutput() const override
    {
        return false;
    };
    std::string toString() const override
    {
        if (isCompleted())
        {
            return "Container has been cleared";
        }
        else
        {
            return "This task clears the container";
        }
    }

private:
    Container<Task> *task_container;
};

class CountObjectsTask : public Task
{
public:
    CountObjectsTask()
    {
        output = 0;
    }
    void execute() override
    {
        if (!completed)
        {
            output = get_objects_count();
            completed = true;
        }
        else
            throw std::runtime_error("Task already done");
    }
    bool hasOutput() const override
    {
        return true;
    };
    std::string toString() const override
    {
        if (isCompleted())
        {
            return "There are " + std::to_string(output) + " objects in program";
        }
        else
        {
            return "This task counts the number of objects in the program";
        }
    }

private:
    int output;
};

#endif