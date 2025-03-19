#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <thread>

class CarDraw : public rclcpp::Node {
public:
    CarDraw() : Node("car_draw"), step_count_(0) {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1), std::bind(&CarDraw::draw, this));
        RCLCPP_INFO_STREAM(this->get_logger(), "Drawing a car in turtlesim.");
        draw();
    }

private:
    void publish_message(double fwd, double turn) {
        auto message = geometry_msgs::msg::Twist();
        message.linear.x = fwd;
        message.angular.z = turn;
        step_count_++;
        RCLCPP_INFO(this->get_logger(), "Step %ld: speed=%.1f, turn=%.1f", step_count_, message.linear.x, message.angular.z);
        publisher_->publish(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void draw() {
        RCLCPP_INFO_STREAM(this->get_logger(), "Starting car drawing.");
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Draw left wheel (small circle)
        for (int i = 0; i < 10; i++) publish_message(0.5, 0.6);
        publish_message(0.0, 1.5);

        // Move to right wheel position
        publish_message(2.0, 0.0);
        publish_message(0.0, -1.5);

        // Draw right wheel (small circle)
        for (int i = 0; i < 10; i++) publish_message(0.5, 0.6);
        publish_message(0.0, 1.5);

        // Move to body start
        publish_message(-1.0, 0.0);
        publish_message(0.0, -1.5);

        // Draw car body (rectangle)
        publish_message(3.0, 0.0);
        publish_message(0.0, 1.57);
        publish_message(1.5, 0.0);
        publish_message(0.0, 1.57);
        publish_message(3.0, 0.0);
        publish_message(0.0, 1.57);
        publish_message(1.5, 0.0);
        publish_message(0.0, 1.57);

        RCLCPP_INFO_STREAM(this->get_logger(), "Car drawing completed.");
        rclcpp::shutdown();
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t step_count_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CarDraw>());
    rclcpp::shutdown();
    return 0;
}
