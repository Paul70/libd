#include "version.h"
#include "tests/testbase.h"
#include <string_view>

//using namespace DUTIL;

namespace {
class VersionTests : public TestBase
{};
}

TEST_F(VersionTests, testIfGitVersionParametersExist)
{
    std::string_view git_version {LIBD_CURRENT_GIT_VERSION};
    std::string_view git_tag {LIBD_LATEST_GIT_TAG};

    EXPECT_THAT(git_version, testing::HasSubstr("git version"));
    EXPECT_THAT(git_tag, testing::HasSubstr("v1.0.0-alpha.1"));
}


