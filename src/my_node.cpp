#include <iostream>
#include <string>
using namespace std;

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>

class Fusion
{
    public:
    Fusion()
    {
        //---- Read params
        ros::NodeHandle private_nh("~");
        private_nh.param("ODOMETRY_TOPIC_NAME", ODOMETRY_TOPIC_NAME, std::string("/base_pose_in_map"));
        private_nh.param("OUTPUT_ODOM_TOPIC_NAME", OUTPUT_ODOM_TOPIC_NAME, std::string("/the_outtopic"));

        // Way-1
        private_nh.param("imu_param/gyro_bias", gyro_bias, -1.0f );
        private_nh.param("imu_param/accelerometer_bias", accelerometer_bias, -1.0f );


        // Way-2 (Preferred)
        private_nh.param<double>("clustTor_search", clustTor_search , -1 );
        private_nh.param<bool>("useTurn", useTurn, false);
        private_nh.param<string>("modeX", modeX, "NA");



        // How to know if a param exists or not
        if( !private_nh.hasParam("a_list") )
            cout << "`a_list` does not exist\n";
        else
            cout << "`a_list` exists\n";


        // How to retrive a list
        vector<double> a_list;
        private_nh.getParam("a_list", a_list );
        cout << "len(a_list) is " << a_list.size() << endl;
        for( auto i=0 ; i<a_list.size() ; i++ )
            cout << "\t" << i << ": " << a_list[i] << "\n";

        cout << "---Loaded---\n";
        cout << "\t gyro_bias:=" << gyro_bias << endl;
        cout << "\t accelerometer_bias:=" << accelerometer_bias << endl;
        cout << "\t clustTor_search:=" << clustTor_search << endl;
        cout << "\t useTurn:=" << useTurn << endl;
        cout << "\t modeX:=" << modeX << endl;

        cout << "---Loaded---\n";


        //---- Subscribes and Publishers
        cout  << "Subscribe ODOMETRY_TOPIC_NAME=" << ODOMETRY_TOPIC_NAME  << endl;
        sub_lidar_odom = nh_.subscribe(ODOMETRY_TOPIC_NAME, 100, &Fusion::lidar_odom_callback, this);

        cout << "Advertise OUTPUT_ODOM_TOPIC_NAME=" << OUTPUT_ODOM_TOPIC_NAME << endl;
        pub_base_pose_in_map_100hz = nh_.advertise<std_msgs::Bool>(OUTPUT_ODOM_TOPIC_NAME, 100);

    }

    //---- callbacks
    void lidar_odom_callback( const std_msgs::Bool::ConstPtr msg )
    {
        cout << "RCVD a CB " << ros::Time::now() << endl;
    }

    private:
    // ros
    ros::NodeHandle nh_;
    ros::Subscriber sub_lidar_odom;
    ros::Publisher pub_base_pose_in_map_100hz;

    // global variables
    float gyro_bias, accelerometer_bias;
    string ODOMETRY_TOPIC_NAME, OUTPUT_ODOM_TOPIC_NAME;
    double clustTor_search;
    bool useTurn;
    string modeX;


    // helper functions
};

int main(int argc, char ** argv )
{
    ros::init(argc, argv, "fusion_node_with_config_files");
    std::cout<<"+++ Fusion Node (With CFG) +++ \n";

    Fusion fusion_;

    ros::spin();
    return 0;
}
