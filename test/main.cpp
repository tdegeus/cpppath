#include <catch2/catch_all.hpp>
#include <cpppath.h>

TEST_CASE("cpppath", "cpppath.h")
{

    SECTION("ext")
    {
        REQUIRE(cpppath::ext(cpppath::join({"path", "to", "foo", "bar.txt"})) == "txt");
        REQUIRE(cpppath::ext("bar.txt") == "txt");
        REQUIRE(cpppath::ext(".bashrc.txt") == "txt");
        REQUIRE(cpppath::ext(".bashrc") == "");
        REQUIRE(cpppath::ext("foo") == "");
    }

    SECTION("splitext")
    {
        std::string path = cpppath::join({"path", "bar"});
        std::string ext = "txt";
        REQUIRE(cpppath::splitext(path + "." + ext) == std::vector<std::string>{path, ext});
        REQUIRE(cpppath::splitext("bar.txt") == std::vector<std::string>{"bar", "txt"});
        REQUIRE(cpppath::splitext(".bashrc.txt") == std::vector<std::string>{".bashrc", "txt"});
        REQUIRE(cpppath::splitext(".bashrc") == std::vector<std::string>{".bashrc", ""});
        REQUIRE(cpppath::splitext("foo") == std::vector<std::string>{"foo", ""});
    }

    SECTION("filebase")
    {
        REQUIRE(cpppath::filebase(cpppath::join({"path", "to", "foo", "bar.txt"})) == "bar");
        REQUIRE(cpppath::filebase("bar.txt") == "bar");
        REQUIRE(cpppath::filebase(".bashrc.txt") == ".bashrc");
        REQUIRE(cpppath::filebase(".bashrc") == ".bashrc");
        REQUIRE(cpppath::filebase("foo") == "foo");
    }

    SECTION("split/join")
    {
        std::vector<std::string> list = {"path", "to", "foo", "bar.txt"};
        REQUIRE(cpppath::split(cpppath::join(list)) == list);
        REQUIRE(cpppath::split(cpppath::join(list, true)) == list);
    }

    SECTION("split")
    {
        std::vector<std::string> list = {"path", "to"};
        int n = (int)list.size();
        std::vector<std::string> post = {"foo", "bar.txt"};
        list.insert(list.end(), post.begin(), post.end());
        REQUIRE(cpppath::split(cpppath::join(list), n) == post);
        REQUIRE(cpppath::split(cpppath::join(list), n, n + 1) == std::vector<std::string>{post[0]});
        REQUIRE(cpppath::split(cpppath::join(list), -2) == post);
        REQUIRE(cpppath::split(cpppath::join(list), -1) == std::vector<std::string>{post[1]});
    }

    SECTION("select")
    {
        std::vector<std::string> list = {"path", "to"};
        int n = (int)list.size();
        std::vector<std::string> post = {"foo", "bar.txt"};
        list.insert(list.end(), post.begin(), post.end());
        REQUIRE(cpppath::select(cpppath::join(list), n) == cpppath::join(post));
        REQUIRE(cpppath::select(cpppath::join(list), n, n + 1) == post[0]);
        REQUIRE(cpppath::select(cpppath::join(list), -2) == cpppath::join(post));
        REQUIRE(cpppath::select(cpppath::join(list), -2, -1) == post[0]);
    }

    SECTION("dirname/filename")
    {
        std::vector<std::string> dirname = {"path", "to", "foo"};
        std::string filename = "bar.txt";
        std::vector<std::string> path = dirname;
        path.push_back(filename);
        REQUIRE(cpppath::split(cpppath::dirname(cpppath::join(path))) == dirname);
        REQUIRE(cpppath::split(cpppath::dirname(cpppath::join(path, true))) == dirname);
        REQUIRE(cpppath::filename(cpppath::join(path)) == filename);
        REQUIRE(cpppath::filename(cpppath::join(path, true)) == filename);
    }

    SECTION("normpath")
    {
        std::vector<std::string> full = {"path", "to", "..", "foo"};
        std::vector<std::string> reduced = {"path", "foo"};
        REQUIRE(cpppath::normpath(cpppath::join(full)) == cpppath::join(reduced));
    }

    SECTION("commonprefix/commondirname")
    {
        std::vector<std::string> prefix = {"path", "to", "foo"};
        std::vector<std::string> a = prefix;
        std::vector<std::string> b = prefix;
        std::vector<std::string> c = prefix;
        a.push_back("bar.txt");
        b.push_back("some");
        b.push_back("more.txt");
        c.push_back("really");
        c.push_back("very");
        c.push_back("long.txt");
        std::string A = cpppath::join(a);
        std::string B = cpppath::join(b);
        std::string C = cpppath::join(c);
        REQUIRE(cpppath::split(cpppath::commonprefix({A, B, C})) == prefix);
        REQUIRE(cpppath::split(cpppath::commondirname({A, B, C})) == prefix);
    }

    SECTION("exist/curdir (compile only)")
    {
        cpppath::curdir();
        cpppath::exists("main.cpp");
    }
}
