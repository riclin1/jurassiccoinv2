#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-jurassic");
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", false));
    BOOST_CHECK(GetBoolArg("-jurassic", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-jurassico"));
    BOOST_CHECK(!GetBoolArg("-jurassico", false));
    BOOST_CHECK(GetBoolArg("-jurassico", true));

    ResetArgs("-jurassic=0");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));

    ResetArgs("-jurassic=1");
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", false));
    BOOST_CHECK(GetBoolArg("-jurassic", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nojurassic");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));

    ResetArgs("-nojurassic=1");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));

    ResetArgs("-jurassic -nojurassic");  // -jurassic should win
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", false));
    BOOST_CHECK(GetBoolArg("-jurassic", true));

    ResetArgs("-jurassic=1 -nojurassic=1");  // -jurassic should win
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", false));
    BOOST_CHECK(GetBoolArg("-jurassic", true));

    ResetArgs("-jurassic=0 -nojurassic=0");  // -jurassic should win
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--jurassic=1");
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", false));
    BOOST_CHECK(GetBoolArg("-jurassic", true));

    ResetArgs("--nojurassic=1");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", "eleven"), "eleven");

    ResetArgs("-jurassic -bar");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", "eleven"), "");

    ResetArgs("-jurassic=");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", "eleven"), "");

    ResetArgs("-jurassic=11");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", "eleven"), "11");

    ResetArgs("-jurassic=eleven");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-jurassic", 0), 0);

    ResetArgs("-jurassic -bar");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-jurassic=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-jurassic=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--jurassic");
    BOOST_CHECK_EQUAL(GetBoolArg("-jurassic"), true);

    ResetArgs("--jurassic=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-jurassic", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nojurassic");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));

    ResetArgs("-nojurassic=1");
    BOOST_CHECK(!GetBoolArg("-jurassic"));
    BOOST_CHECK(!GetBoolArg("-jurassic", true));
    BOOST_CHECK(!GetBoolArg("-jurassic", false));

    ResetArgs("-nojurassic=0");
    BOOST_CHECK(GetBoolArg("-jurassic"));
    BOOST_CHECK(GetBoolArg("-jurassic", true));
    BOOST_CHECK(GetBoolArg("-jurassic", false));

    ResetArgs("-jurassic --nojurassic");
    BOOST_CHECK(GetBoolArg("-jurassic"));

    ResetArgs("-nojurassic -jurassic"); // jurassic always wins:
    BOOST_CHECK(GetBoolArg("-jurassic"));
}

BOOST_AUTO_TEST_SUITE_END()
