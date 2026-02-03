#include <gtest/gtest.h>
#include <stdexec/execution.hpp>

#include "mandelbrot_sender.hpp"
#include "sfml_events_handler.hpp"

namespace ex = stdexec;

TEST(ViewPortTest, WidthAndHeight) {
    ViewPort vp{-2.5, 1.5, -2.0, 2.0};

    EXPECT_DOUBLE_EQ(vp.width(), 4.0);
    EXPECT_DOUBLE_EQ(vp.height(), 4.0);
}

TEST(AvrTimeCounterTest, AverageIncreases) {
    AvrTimeCounter counter;

    counter.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    counter.End();

    counter.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    counter.End();

    EXPECT_EQ(counter.Count(), 2);
    EXPECT_GT(counter.GetAvr(), 0);
}

TEST(MandelbrotComputeSenderTest, ProducesFrameBuffer) {
    RenderSettings settings{.width = 10, .height = 20, .max_iterations = 30, .escape_radius = 2.0};
    ViewPort vp;

    auto sender = mandelbrot::MakeComputeSender(settings, vp);

    auto result = ex::sync_wait(sender);
    ASSERT_TRUE(result.has_value());

    auto fb = *result;
    ASSERT_NE(std::get<0>(fb), nullptr);
    EXPECT_EQ(std::get<0>(fb)->rgba.size(), 10 * 20 * 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
