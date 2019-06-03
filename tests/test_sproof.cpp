
#include <memory>

#include <sproof-utils/sproof-utils.h>

#include "config.h"
#include "sproof.h"


static sproof::Credentials testcredentials{
    "", "", "", ""
};

static sproof::Config testconfig{
    "testurl",
    testcredentials
};


void test_add_event();

void test_register_profile();

void test_revoke_profile();

void test_confirm_profile();

void test_update_profile();

void test_register_document();

void test_revoke_document();

void test_add_document_receiver();

void test_revoke_document_receiver();

int main(int argc, char** argv)
{
    test_add_event();
    test_register_profile();
    test_revoke_profile();
    test_confirm_profile();
    test_update_profile();
    test_register_document();
    test_revoke_document();
    test_add_document_receiver();
    test_revoke_document_receiver();

    return 0;
}

void test_revoke_document_receiver() {

    TEST_BEGIN("test_revoke_document_receiver");

    sproof::Sproof sproof{testconfig};

    sproof.revoke_document_receiver("0xcafec0de", "some reason");

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("DOCUMENT_RECEIVER_REVOKE", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("0xcafec0de", events[0]->to_json()["data"]["receiverId"].asString());
    ASSERT_STRING_EQUAL("some reason", events[0]->to_json()["data"]["reason"].asString());

    TEST_END();

}

void test_add_document_receiver() {

    TEST_BEGIN("test_add_document_receiver");

    sproof::Sproof sproof{testconfig};

    sproof.add_document_receiver("0xcafec0de", "0xfeedbeef");

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("DOCUMENT_RECEIVER_ADD", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("0xcafec0de", events[0]->to_json()["data"]["receiverId"].asString());
    ASSERT_STRING_EQUAL("0xfeedbeef", events[0]->to_json()["data"]["documentHash"].asString());

    TEST_END();

}

void test_revoke_document() {

    TEST_BEGIN("test_revoke_document");

    sproof::Sproof sproof{testconfig};

    sproof.revoke_document("0xfeedbeef", "not good enough");

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("DOCUMENT_REVOKE", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("0xfeedbeef", events[0]->to_json()["data"]["documentHash"].asString());
    ASSERT_STRING_EQUAL("not good enough", events[0]->to_json()["data"]["reason"].asString());

    TEST_END();

}

void test_register_document() {

    TEST_BEGIN("test_register_document");

    sproof::Sproof sproof{testconfig};

    sproof::Document document;
    document.valid_from = 10l;
    document.valid_until = 20l;
    document.name = "testdoc";
    document.document_hash = "0xdeadbeef";

    sproof.register_document(document);

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("DOCUMENT_REGISTER", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("testdoc", events[0]->to_json()["data"]["name"].asString());
    ASSERT_STRING_EQUAL("0xdeadbeef", events[0]->to_json()["data"]["documentHash"].asString());
    ASSERT_EQUAL(10l, events[0]->to_json()["data"]["validFrom"].asInt64());
    ASSERT_EQUAL(20l, events[0]->to_json()["data"]["validUntil"].asInt64());

    TEST_END();

}

void test_update_profile() {

    TEST_BEGIN("test_update_profile");

    sproof::Sproof sproof{testconfig};

    sproof::Profile profile;
    profile.name = "testname";
    profile.website = "testsite.com";

    sproof.update_profile(profile);

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("PROFILE_UPDATE", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("testname", events[0]->to_json()["data"]["name"].asString());
    ASSERT_STRING_EQUAL("testsite.com", events[0]->to_json()["data"]["website"].asString());

    TEST_END();

}

void test_confirm_profile() {

    TEST_BEGIN("test_confirm_profile");

    sproof::Sproof sproof{testconfig};

    sproof.confirm_profile("profile-id", true);
    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("PROFILE_CONFIRM", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("profile-id", events[0]->to_json()["data"]["profileId"].asString());
    ASSERT_EQUAL(true, events[0]->to_json()["data"]["value"].asBool());

    TEST_END();

}

void test_revoke_profile() {

    TEST_BEGIN("test_revoke_profile");

    sproof::Sproof sproof{testconfig};

    sproof.revoke_profile("testreason");
    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("PROFILE_REVOKE", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("testreason", events[0]->to_json()["data"]["reason"].asString());

    TEST_END();

}

void test_register_profile() {

    TEST_BEGIN("test_register_profile");

    sproof::Sproof sproof{testconfig};

    sproof::Profile profile;
    profile.name = "testname";

    sproof.register_profile(profile);

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("PROFILE_REGISTER", events[0]->get_event_name());
    ASSERT_STRING_EQUAL("testname", events[0]->to_json()["data"]["name"].asString());

    TEST_END();
}

void test_add_event() {

    TEST_BEGIN("test_add_event");

    sproof::Sproof sproof{testconfig};

    sproof::Profile profile;
    profile.name = "testname";
    std::unique_ptr<sproof::SproofEvent> event{std::make_unique<sproof::ProfileRegister>(profile)};
    sproof.add_event(std::move(event));

    auto& events = sproof.get_events();
    ASSERT_EQUAL(1, events.size());
    ASSERT_STRING_EQUAL("PROFILE_REGISTER", events[0]->get_event_name());

    TEST_END();

}
