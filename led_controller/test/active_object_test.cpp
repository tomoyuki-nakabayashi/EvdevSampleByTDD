// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <gmock/gmock.h>
#include <memory>
#include <active_object_engine.h>
#include <command/command.h>
#include <command/action_on_trigger.h>
#include <detector/event_detector.h>
#include <detector/timeout_detector.h>

namespace led_controller_test {

typedef struct EvenCountDetector {
  EventDetectorStruct base;
  uint32_t my_data;
} EvenCountDetector;

static int ClearEvenCounter(EventDetector super) {
  auto self = reinterpret_cast<EvenCountDetector*>(super);
  self->my_data = 0;
  return DETECTOR_SUCCESS;
}

static int CheckEvenCounter(EventDetector super) {
  auto self = reinterpret_cast<EvenCountDetector*>(super);
  self->my_data++;
  return ((self->my_data % 2) == 0) ?
    DETECTOR_EVENT_DETECTED : DETECTOR_EVENT_NOT_DETECTED;
}

static EventDetectorInterfaceStruct even_interface = {
  ClearEvenCounter, CheckEvenCounter, ClearEvenCounter,
};

static EventDetector CreateEvenCountDetector() {
  auto detector = new EvenCountDetector{};
  detector->base.vtable = &even_interface;
  return reinterpret_cast<EventDetector>(detector);
}

typedef struct CountCommand {
  CommandStruct base;
  int32_t my_data;
} CountCommand;

static void IncrementCounter(Command super) {
  auto self = reinterpret_cast<CountCommand*>(super);
  self->my_data++;
}

static CommandInterfaceStruct count_interface = {
  IncrementCounter
};

static Command CreateIncrementCounter() {
  auto command = new CountCommand{};
  command->base.vtable = &count_interface;
  return reinterpret_cast<Command>(command);
}

typedef struct TotalCountCommand {
  CommandStruct base;
  int32_t total_counter;
} TotalCountCommand;

static void IncrementTotalCounter(Command super) {
  auto self = reinterpret_cast<TotalCountCommand*>(super);
  self->total_counter++;
}

static CommandInterfaceStruct total_counter_interface = {
  IncrementTotalCounter
};

static Command CreateTotalCount() {
  auto cmd = new TotalCountCommand{};
  cmd->base.vtable = &total_counter_interface;
  cmd->total_counter = 0;
  return reinterpret_cast<Command>(cmd);
}

class ActiveObjectEngineTest : public ::testing::Test {
 protected:
    ActiveObjectEngineTest()
        : detector_{CreateEvenCountDetector()}
        , op_{CreateIncrementCounter()}
        , count_even_{CreateTriggerActionPair(detector_.get(), op_.get())} {
    }

    void SetUp() override {
      actions_ = new TriggerActionPair[3];
      actions_[0] = count_even_;
      actions_[1] = count_even_;
      actions_[2] = nullptr;
    }

    void TearDown() override {
      delete[] actions_;
      DestroyTriggerActionPair(count_even_);
    }

 protected:
    std::unique_ptr<EventDetectorStruct> detector_;
    std::unique_ptr<CommandStruct> op_;
    TriggerActionPair count_even_;
    TriggerActionPair *actions_;
};
/* 
TEST_F(ActiveObjectEngineTest, AbstractUse) {
  auto engine = CreateActiveObjectEngine();
  auto cmd = CreateActionOnTriggerChain(actions_);
  FuelEngine(engine, cmd);
  EngineRuns(engine);
}
 */

TEST_F(ActiveObjectEngineTest, OneCommandTest) {
  auto engine = CreateActiveObjectEngine();
  auto cmd = CreateTotalCount();
  FuelEngine(engine, cmd);
  EngineRuns(engine);

  EXPECT_EQ(1, reinterpret_cast<TotalCountCommand*>(cmd)->total_counter);
  DestroyActiveObjectEngine(engine);
}

TEST_F(ActiveObjectEngineTest, MultipleCommandsTest) {
  auto engine = CreateActiveObjectEngine();
  auto cmd = CreateTotalCount();
  FuelEngine(engine, cmd);
  FuelEngine(engine, cmd);
  FuelEngine(engine, cmd);
  EngineRuns(engine);

  EXPECT_EQ(3, reinterpret_cast<TotalCountCommand*>(cmd)->total_counter);
  DestroyActiveObjectEngine(engine);
}

TEST_F(ActiveObjectEngineTest, FuelActionOnTrigger) {
  auto engine = CreateActiveObjectEngine();
  auto cmd = CreateActionOnTriggerChain(actions_, engine);
  FuelEngine(engine, cmd);
  EngineRuns(engine);

  EXPECT_EQ(2, reinterpret_cast<CountCommand*>(op_.get())->my_data);
}

}  // namespace led_controller_test
