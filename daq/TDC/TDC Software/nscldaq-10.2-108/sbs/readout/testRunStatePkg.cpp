// Template for a test suite.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"
#include "RunState.h"
#include "CReadoutMain.h"
#include "CNullTrigger.h"
#include "CExperiment.h"
#include <TCLInterpreter.h>
#include "RunState.h"
#include "CRunControlPackage.h"
#include <StateException.h>
#include <TCLException.h>
#include <string>

class CTCLInterpreter;

using namespace std;

// we have to pray that our main will overide the one in TCLApplication.cpp for any of this
// to work.

CTCLApplication* gpTCLApplication(0);




//////////////////////////////////////////////////////////////////////
//
// Test harness.. I need to make enough of a fake  CReadoutMain
//                to provide the CExperiment...also need to set the
//                experiment trigger.
//

static const std::string testRingName = "testrunctlpkg";


// Dummy implementation of CReadoutMain:
// This leaks a pair of null trigger objects per test which is acceptable for testing.

CReadoutMain::CReadoutMain() :
  m_pExperiment(new CExperiment(testRingName))
{
  m_pExperiment->EstablishTrigger(new CNullTrigger);
  m_pExperiment->setScalerTrigger(new CNullTrigger);
}
CReadoutMain::~CReadoutMain()
{
  delete m_pExperiment;
}
CExperiment*
CReadoutMain::getExperiment()
{
  return m_pExperiment;
}

// truly stubs:
int
CReadoutMain::operator()() { return 0; }
CExperiment* 
CReadoutMain::CreateExperiment(void * parsed) { return m_pExperiment; }
void
CReadoutMain::SetupRunVariables(CTCLInterpreter* pInterp) {}
void
CReadoutMain::SetupStateVariables(CTCLInterpreter* pInterp) {}
void 
CReadoutMain::SetupScalers(CExperiment* p) {}
void
CReadoutMain::SetupReadout(CExperiment* p) {}
void 
CReadoutMain::addCommands() {}

void CReadoutMain::addCommands(CTCLInterpreter* p) {}


class ReadoutTestHarness : public CReadoutMain
{
protected:
  virtual ~ReadoutTestHarness() {}
  virtual void SetupReadout(CExperiment* pExperiment) {}
};


class TestRctlPackage : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestRctlPackage);
  CPPUNIT_TEST(begin);
  CPPUNIT_TEST(pause);
  CPPUNIT_TEST(resume);
  CPPUNIT_TEST(end);
  CPPUNIT_TEST(begincommand);
  CPPUNIT_TEST(pausecommand);
  CPPUNIT_TEST(resumecommand);
  CPPUNIT_TEST(endcommand);
  CPPUNIT_TEST_SUITE_END();


private:
  static CTCLInterpreter* m_pInterp;
  static CReadoutMain*    m_pReadout;
  RunState*        m_pRunState;
  CExperiment*     m_pExperiment;
public:
  void setUp() {
    if (!m_pInterp) {
      m_pInterp        = new CTCLInterpreter();
      m_pReadout       = new ReadoutTestHarness;
      gpTCLApplication = m_pReadout;
  
    }
    RunState* pState = RunState::getInstance();
    pState->m_state      = RunState::inactive;
    pState->m_runNumber  = 0;
    pState->m_timeOffset = 0;
    pState->m_pTitle     = const_cast<char*>("Test title");

    m_pRunState = pState;
    m_pExperiment = m_pReadout->getExperiment();
    
  }
  void tearDown() {

  }
protected:
  void begin();
  void pause();
  void resume();
  void end();
  void begincommand();
  void pausecommand();
  void resumecommand();
  void endcommand();
};

CTCLInterpreter* TestRctlPackage::m_pInterp(0);
CReadoutMain*    TestRctlPackage::m_pReadout(0);


CPPUNIT_TEST_SUITE_REGISTRATION(TestRctlPackage);

///////////////////////////////////////////////////////////////////////
//
// Tests:


//
// Begin run.
// There are two begin run cases.
// The first is a valid begin run (state is inactive.
// the second is invalid run state which should throw a state exception
//
void TestRctlPackage::begin() 
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);
  pkg->begin();

  EQ(RunState::active, m_pRunState->m_state);
  
  // Should throw to start a new run:


  bool threw = false;
  bool threwstate = false;
  try {
    pkg->begin();
  }
  catch (CStateException& e) {
    threwstate = true;
    threw      = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(threwstate);

  // Stop the run as part of the cleanup:

  m_pExperiment->Stop();


}
// Legal to pause the run only when active:
//
void TestRctlPackage::pause()
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);

  // we know that begin works.. after a begin, a pause should be legal:

  pkg->begin();
  pkg->pause();

  EQ(RunState::paused, m_pRunState->m_state);

  // Should not be able to pause again:

  bool threw = false;
  bool state = false;

  try {
    pkg->pause();
  }
  catch (CStateException& e) {
    threw = true;
    state = true;
  }
  catch(...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(state);

  // If the run state is halted, we still should not be able
  // to pause the run:

  m_pExperiment->Stop();	// End the run in the experiment
  m_pRunState->m_state = RunState::inactive;

  threw = false;
  state = false;

  try {
    pkg->pause();
  }
  catch (CStateException& e) {
    threw = true;
    state = true;
  }
  catch(...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(state);



}

// Legal to resume the run only when paused:

void TestRctlPackage::resume()
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);

  // Run is halted should not be legal to resume.

  bool threw = false;
  bool state = false;

  try {
    pkg->resume();
  }
  catch (CStateException& e) {
    threw = true;
    state = true;
  }
  catch(...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(state);

  // Should not be legal to resume the run when it's active either:

  pkg->begin();
  threw = false;
  state = false;
  try {
    pkg->resume();
  }
  catch (CStateException& e) {
    threw = true;
    state = true;
  }
  catch(...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(state);

  // After a pause resume is legal:

  pkg->pause();
  pkg->resume();
  EQ(RunState::active, m_pRunState->m_state);

  // Clean up by ending the run:

  m_pExperiment->Stop();


  
}


// Legal to end the run when active or paused.
//
void TestRctlPackage::end()
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);

  pkg->begin();			//  end is legal when active:
  pkg->end();
  EQ(RunState::inactive, m_pRunState->m_state);

  pkg->begin(); pkg->pause();	// End is legal when paused:
  pkg->end();
  EQ(RunState::inactive, m_pRunState->m_state);

  // when run is inactive, it's illegal to stop it again:

  bool threw = false;
  bool state = false;

  try {
    pkg->end();
  }
  catch (CStateException& e) {
    threw = true;
    state = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(state);

}
//
// Check the begin command.. Should start the run if inactive.
// should toss a CTCLException if the runis not inactive.
//
void TestRctlPackage::begincommand()
{
  CRunControlPackage *pkg = CRunControlPackage::getInstance(*m_pInterp);
  string              result;
  bool                threw = false; // Used to flag some exception thrown.
  bool                tcl   = false; // Used to flag a tcl exception thrown.

  result = m_pInterp->Eval("begin");
  EQ(RunState::active, m_pRunState->m_state);

  // Doing a begin again should fail:

  try {
    result = m_pInterp->Eval("begin");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

  pkg->pause();
  threw  = false;
  tcl    = false;

  // Doing a begin again should fail:

  try {
    result = m_pInterp->Eval("begin");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

  pkg->end();			// end the run.

}
//
// Check the pause command it operates under the same constraints
// as the pause operation directly applied to the package but
// the exceptions will be CTCLExceptions not CStateExceptions.
//
void TestRctlPackage::pausecommand()
{
  CRunControlPackage *pkg = CRunControlPackage::getInstance(*m_pInterp);
  string              result;
  bool                threw = false;
  bool                tcl   = false;

  result = m_pInterp->Eval("begin"); // start the run.

  // Pause should be allowed:

  result = m_pInterp->Eval("pause");
  EQ(RunState::paused, m_pRunState->m_state);
  
  // Pause should not be allowed in the pause state:

  try {
    result = m_pInterp->Eval("pause");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

  pkg->end();
  threw = false;
  tcl   = false;

  // Pause not allowed in the inactive state too:'

  try {
    result = m_pInterp->Eval("pause");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

}

// Check the resume command.  It operates under the same constraints
// as the resuem operation, except that any exceptions will be
// CTCLException s rather than CStateException.
//
void TestRctlPackage::resumecommand()
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);
  string              result;
  bool                threw = false;
  bool                tcl   = false;

  result = m_pInterp->Eval("begin");
  result = m_pInterp->Eval("pause"); // Now resume is legal:

  result = m_pInterp->Eval("resume");
  EQ(RunState::active, m_pRunState->m_state);

  // Now let's try some illegals:
  // Can't resum an active run:
  try {
    result = m_pInterp->Eval("resume");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

  // Can't resume an inactive run:

  pkg->end();
  threw = false;
  tcl   = false;

  try {
    result = m_pInterp->Eval("resume");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

}
// Check the end command. It operates under the same constraints
// as the end operation.  Exceptions are CTCLExceptions.
//
void TestRctlPackage::endcommand()
{
  CRunControlPackage* pkg = CRunControlPackage::getInstance(*m_pInterp);
  string              result;
  bool                threw = false;
  bool                tcl   = false;

  result = m_pInterp->Eval("begin"); // end is legal now:

  result = m_pInterp->Eval("end");
  EQ(RunState::inactive, m_pRunState->m_state);

  // After the run is inactive, end is not legal:

  try {
    result = m_pInterp->Eval("end");
  }
  catch (CTCLException& e) {
    threw = true;
    tcl   = true;
  }
  catch (...) {
    threw = true;
  }
  ASSERT(threw);
  ASSERT(tcl);

  // Paused runs, on the other hand, can be ended:

  pkg->begin();
  pkg->pause();
  result = m_pInterp->Eval("end");
  EQ(RunState::inactive, m_pRunState->m_state);

}
