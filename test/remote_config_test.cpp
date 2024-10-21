#include "gtest/gtest.h"
#include "remote_config.h"

using namespace std;

TEST(RemoteConfigTest, GetConfigTest) {

    RemoteConfig remoteConfig;
    std::string name;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < 100; i++)
    {
        auto config = remoteConfig.GetConfig("test");
        name = config.name;
    }

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Elapsed time: " << duration.count() << " seconds\n";

    ASSERT_EQ(duration.count() < 0.1, true);
    cout<<name<<endl;

}