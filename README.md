# jsonlib

A simple JSON parsing header library

## Usage

```cpp
    Json obj;
    obj["null"] = nullptr;
    auto ret = obj.serialize(); // {"null": null}
```

```cpp
    Json obj;
    obj["bool1"] = true;
    obj["bool2"] = false;
    auto ret = obj.serialize(); // {"bool1": true, "bool2": false}
```

```cpp
    Json obj;
    obj["number1"] = 1;
    obj["number2"] = 3.14;
    auto ret = obj.serialize(); // {"number1": 1, "number2": 3.14}
```

```cpp
    Json obj;
    obj["hello"] = "world";
    obj["empty"] = "";
    auto ret = obj.serialize(); // {"empty": "", "hello": "world"}
```

```cpp
    Json obj;
    obj["array"] = {"R", "G", "B"};
    obj["empty"] = {};
    auto ret = obj.serialize(); // {"array": ["R", "G", "B"], "empty": null}
```

```cpp
    Json obj;
    obj["array"] = {"string", 100, true};
    auto ret = obj.serialize(); // {"array": ["string", 100, true]}
```

```cpp
    Json obj;
    Json nested_obj1;
    Json nested_obj2;
    obj["level1"] = nested_obj1;
    nested_obj1["level2"] = nested_obj2;
    nested_obj2["level3"] = Json();
    auto ret = obj.serialize(); // {"level1": {"level2": {"level3": null}}}
```

## TODO

- [ ] streaming parser
- [ ] error handing
- [ ] compile time reflection for structs
- [ ] useful extensions
