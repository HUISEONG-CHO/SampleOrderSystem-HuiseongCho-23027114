#include <gtest/gtest.h>
#include <filesystem>
#include "repository/SampleRepository.h"

namespace fs = std::filesystem;

class SampleRepositoryTest : public ::testing::Test {
protected:
    const std::string testFile = "test_samples.json";
    void SetUp()    override { if (fs::exists(testFile)) fs::remove(testFile); }
    void TearDown() override { if (fs::exists(testFile)) fs::remove(testFile); }
    SampleRepository makeRepo() { return SampleRepository(testFile); }
};

TEST_F(SampleRepositoryTest, SaveAndFindAll) {
    auto repo = makeRepo();
    repo.save(Sample("S001", "SampleA", 30, 0.85, 100));
    repo.save(Sample("S002", "SampleB", 45, 0.90, 200));
    auto all = repo.findAll();
    ASSERT_EQ(all.size(), 2u);
    EXPECT_EQ(all[0].getId(), "S001");
    EXPECT_EQ(all[1].getId(), "S002");
}

TEST_F(SampleRepositoryTest, FindById) {
    auto repo = makeRepo();
    repo.save(Sample("S001", "SampleA", 30, 0.85, 100));
    Sample s = repo.findById("S001");
    EXPECT_EQ(s.getName(), "SampleA");
    EXPECT_EQ(s.getStock(), 100);
}

TEST_F(SampleRepositoryTest, Update) {
    auto repo = makeRepo();
    repo.save(Sample("S001", "SampleA", 30, 0.85, 100));
    Sample updated("S001", "SampleA", 30, 0.85, 999);
    repo.update("S001", updated);
    EXPECT_EQ(repo.findById("S001").getStock(), 999);
}

TEST_F(SampleRepositoryTest, Remove) {
    auto repo = makeRepo();
    repo.save(Sample("S001", "SampleA", 30, 0.85, 100));
    repo.save(Sample("S002", "SampleB", 45, 0.90, 200));
    repo.remove("S001");
    ASSERT_EQ(repo.findAll().size(), 1u);
    EXPECT_EQ(repo.findAll()[0].getId(), "S002");
}

TEST_F(SampleRepositoryTest, FindByIdThrows) {
    auto repo = makeRepo();
    EXPECT_THROW(repo.findById("NONE"), std::runtime_error);
}

TEST_F(SampleRepositoryTest, FileSaved) {
    auto repo = makeRepo();
    repo.save(Sample("S001", "SampleA", 30, 0.85, 100));
    EXPECT_TRUE(fs::exists(testFile));
}

TEST_F(SampleRepositoryTest, Persistence) {
    { SampleRepository r(testFile); r.save(Sample("S001", "SampleA", 30, 0.85, 100)); }
    { SampleRepository r(testFile); EXPECT_EQ(r.findAll().size(), 1u); }
}
