#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <thread>

class StarDraw : public rclcpp::Node {
public:
    StarDraw() : Node("star_draw"), step_count_(0) {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1), std::bind(&StarDraw::draw, this));
        RCLCPP_INFO_STREAM(this->get_logger(), "Drawing a star in Turtlesim.");
        draw();
    }

private:
    void publish_message(double fwd, double turn, double duration) {
        auto message = geometry_msgs::msg::Twist();
        message.linear.x = fwd;
        message.angular.z = turn;
        step_count_++;
        RCLCPP_INFO(this->get_logger(), "Step %ld: speed=%.2f, turn=%.2f", step_count_, message.linear.x, message.angular.z);
        publisher_->publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(duration * 1000)));
    }

    void draw() {
        RCLCPP_INFO_STREAM(this->get_logger(), "Starting star drawing.");
        std::this_thread::sleep_for(std::chrono::seconds(2));

        double edge_length = 2.0;
        double turn_angle = 144.0 * (M_PI / 180.0);

        for (int i = 0; i < 5; i++) {
            publish_message(edge_length, 0.0, 1.5);
            publish_message(0.0, turn_angle, 1.0);
        }

        RCLCPP_INFO_STREAM(this->get_logger(), "Star drawing completed.");
        rclcpp::shutdown();
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t step_count_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StarDraw>());
    rclcpp::shutdown();
    return 0;
}