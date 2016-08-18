

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <stdint.h>
#include <tuple>

#include "at86rf212/at86rf212.hpp"
#include "mock_spi.hpp"

using namespace AT86RF212;
using namespace testing;

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;

// The fixture for testing class Foo.
class At86rf212Test : public ::testing::Test
{
protected:

  void SetUp()
  {
    radio = At86rf212();

    int res = radio.init(&mockSpi);

    EXPECT_EQ(0, res);
  }

  void TearDown()
  {
    radio.close();
  }

  MockSpi mockSpi;
  At86rf212 radio;
};

TEST_F(At86rf212Test, FakeTest)
{
  // Expected, actual
  EXPECT_EQ(0, 0);
}



