#include <string_view>
#include "tests/testbase.h"
#include "version.h"

//using namespace DUTIL;

namespace {
class VersionTests : public TestBase
{};
}  // namespace

TEST_F(VersionTests, testIfGitVersionParametersExist)
{
  std::string_view git_version{LIBD_CURRENT_GIT_VERSION};

  EXPECT_THAT(git_version, testing::HasSubstr("git version"));
}
