# Remote Config Library for c++

example:
```c++
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

cout<<name<<endl;
```