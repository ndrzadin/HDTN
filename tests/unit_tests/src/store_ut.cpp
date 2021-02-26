#include <arpa/inet.h>
#include <sys/time.h>
#include <iostream>
#include <store.hpp>
#include <zmq.hpp>
#include <boost/test/unit_test.hpp>

std::string GetEnv(const std::string &var) {
    const char *val = std::getenv(var.c_str());
    if (val == nullptr) {  // invalid to assign nullptr to std::string
        return "";
    }
    else {
        return val;
    }
}

/******************************************************************************
 * Checks to see if a directory exists. Note: This method only checks the
 * existence of the full path AND if path leaf is a dir.
 *
 * @return  >0 if dir exists AND is a dir,
 *           0 if dir does not exist OR exists but not a dir,
 *          <0 if an error occurred (errno is also set)
 *****************************************************************************/
//int DirExists(const char *const path) {
//    struct stat info;

//    int statRC = stat(path, &info);
//    if (statRC != 0) {
//        if (errno == ENOENT) {
//            return 0;
//        }  // something along the path does not exist
//        if (errno == ENOTDIR) {
//            return 0;
//        }  // something in path prefix is not a dir
//        return -1;
//    }

//    return (info.st_mode & S_IFDIR) ? 1 : 0;
//}

void stopRegistrationService() {
    // Stop the registration service
    std::string target = "tcp://127.0.0.1:10140";
    std::string svc = "test";
    uint16_t port = 10140;
    std::string mode = "PUSH";
    zmq::context_t *_zmq_ctx = new zmq::context_t;
    zmq::socket_t *_zmq_sock = new zmq::socket_t(*_zmq_ctx, zmq::socket_type::req);
    char tbuf[255];
    memset(tbuf, 0, 255);
    snprintf(tbuf, 255, "%s:%d:%s", svc.c_str(), port, mode.c_str());
    _zmq_sock->setsockopt(ZMQ_IDENTITY, (void *)tbuf, target.size());
    _zmq_sock->connect(target);
    _zmq_sock->send("SHUTDOWN", strlen("SHUTDOWN"), 0);
    _zmq_sock->close();
    delete _zmq_sock;
    delete _zmq_ctx;
    std::cout << " <<<<< Stopped the registration service." << std::endl;
}

// Create a test fixture
class StorageFixture  {
public:
    StorageFixture();
    ~StorageFixture();
    static void SetUpTestCase();     // This is called once before all test cases start.
    static void TearDownTestCase();  // This is called once after all test cases
                                     // have completed.
    static bool staticSetupWorked;
private:
    hdtn::storage *ptrStorage;
    hdtn::storage_worker *ptrStorageWorker;
    //hdtn::Scheduler *ptrScheduler;
};

bool StorageFixture::staticSetupWorked = false;

void StorageFixture::SetUpTestCase() {
    //    staticSetupWorked = false;
    //    std::cout << " >>>>> Called StorageFixture::SetUpTestCase()" <<
    //    std::endl;

    //    // Clean up in case things may have been left running
    //    //stopRegistrationService();
    //    system("killall hdtn-egress");
    //    system("killall hdtn-egress");

    //    std::string hdtnSourceRoot = GetEnv("HDTN_SOURCE_ROOT");
    //    std::cout << " hdtnSourceHome = " << hdtnSourceRoot << std::endl;

    //    std::string hdtnBuildRoot = GetEnv("HDTN_BUILD_ROOT");
    //    std::cout << " hdtnBuildRoot = " << hdtnBuildRoot << std::endl;

    //    // Start the registration service
    //    std::string commandStartReg = "python3 " + hdtnSourceRoot +
    //    "/common/regsvr/main.py &"; system(commandStartReg.c_str()); std::cout
    //    << " >>>>> Started the registration service." << std::endl;

    //    // Start ingress
    //    std::string commandStartIngress = hdtnBuildRoot +
    //    "/module/ingress/hdtn-ingress &"; system(commandStartIngress.c_str());
    //    std::cout << " >>>>> Started the ingress service." << std::endl;

    //    // Start egress
    //    std::string commandStartEgress = hdtnBuildRoot +
    //    "/module/egress/hdtn-egress &"; system(commandStartEgress.c_str());
    //    std::cout << " >>>>> Started the egress service." << std::endl;

    //    staticSetupWorked = true;
}

void StorageFixture::TearDownTestCase() {
    //    std::cout << " <<<<< Called StorageFixture::TearDownTestCase()" <<
    //    std::endl;

    //    // Stop the registration service
    //    stopRegistrationService();

    //    // Clean up in case things could not be shutdown easily
    //    system("killall hdtn-ingress");
    //    system("killall hdtn-egress");
}

StorageFixture::StorageFixture() {
    //    std::cout << "Called StorageFixture::StorageFixture()" << std::endl;
    ptrStorage = new hdtn::storage();
    ptrStorageWorker = new hdtn::storage_worker();
    //ptrScheduler = new hdtn::Scheduler();
}

StorageFixture::~StorageFixture() {
    //    std::cout << "Called StorageFixture::~StorageFixture()" << std::endl;
    delete ptrStorage;
    delete ptrStorageWorker;
    //delete ptrScheduler;
}

//TEST_F(StorageFixture, DISABLED_Init_Update_Stats) {
//    ASSERT_TRUE(staticSetupWorked) << "Error setting up test suite.";

//    hdtn::HdtnRegsvr regsvr;
//    regsvr.Init("tcp://127.0.0.1:10140", "test", 10141, "PUSH");
//    regsvr.Reg();

//    double last = 0.0;
//    timeval tv;
//    gettimeofday(&tv, NULL);
//    last = (tv.tv_sec + (tv.tv_usec / 1000000.0));
//    hdtn::StorageConfig config;
//    config.regsvr = "tcp://127.0.0.1:10140";
//    config.local = "tcp://127.0.0.1:10145";
//    config.storePath = "/tmp/hdtn.store";
//    hdtn::Storage store;

//    std::cout << "[store] Initializing storage manager ..." << std::endl;
//    bool returnValue = store.Init(config);
//    ASSERT_EQ(true, returnValue);

//    store.Update();
//    hdtn::StorageStats *stats = store.Stats();
//    uint64_t cbytes = stats->inBytes;
//    uint64_t ccount = stats->inMsg;
//    printf("[store] Received: %d msg / %0.2f MB\n", ccount, cbytes / (1024.0 * 1024.0));
//    regsvr.Dereg();
//}

//TEST_F(StorageFixture, DISABLED_DispatchTestNominal1) {
//    ASSERT_TRUE(staticSetupWorked) << "Error setting up test suite.";
//    FAIL() << "Test needed for class storage, method dispatch.";
//}

//TEST_F(StorageFixture, DISABLED_C2TelemTestNominal1) {
//    ASSERT_TRUE(staticSetupWorked) << "Error setting up test suite.";
//    FAIL() << "Test needed for class storage, method c2telem.";
//}

//TEST_F(StorageFixture, DISABLED_ReleaseTestNominal1) { FAIL() << "Test needed for class storage, method release."; }

//TEST_F(StorageFixture, DISABLED_IngressTestNominal1) { FAIL() << "Test needed for class storage, method ingress."; }

//TEST_F(StorageFixture, DISABLED_InitWorkerTestNominal1) {
//    FAIL() << "Test needed for class storage_worker, method init.";
//}

//TEST_F(StorageFixture, DISABLED_LaunchTestNominal1) {
//    FAIL() << "Test needed for class storage_worker, method launch.";
//}

//TEST_F(StorageFixture, DISABLED_ExecuteTestNominal1) {
//    FAIL() << "Test needed for class storage_worker, method execute.";
//}

//TEST_F(StorageFixture, DISABLED_ThreadTestNominal1) {
//    FAIL() << "Test needed for class storage_worker, method thread.";
//}

//TEST_F(StorageFixture, DISABLED_WriteTestNominal1) { FAIL() << "Test needed for class storage_worker, method write."; }

//TEST_F(StorageFixture, DISABLED_InitSchedulerTestNominal1) {
//    FAIL() << "Test needed for class scheduler, method init.";
//}

//TEST_F(StorageFixture, DISABLED_AddTestNominal1) { FAIL() << "Test needed for class scheduler, method add."; }

//TEST_F(StorageFixture, DISABLED_NextTestNominal1) { FAIL() << "Test needed for class scheduler, method next."; }



