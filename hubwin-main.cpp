#define ecl false
#include "hub.h"
#include "api/cnet.h"
#include "rich-presence.h"
CanvasCB cbcg::Popup() {}
using hubstate = UserInstance::HubState;


using namespace std;
CNet net;


CanvasCB cbcg::ServerWebInterface() {

   gmElements gme;
   if (GetUser().GetHubState() == hubstate::Logged) {


      gme.designateFamily(
         " name-'product page' y.150 top.40 x.30 x.10 w.150 font.1 rounding.5 ",
         {"v.text name.notekey <text>ACTIVATE KEYS HERE</text> x.10 y.20",
          "v.input name.##key-input input.size.100 h.100",
          "v.button name.activate y.70 w.-50 x.25 "

         }
      );
   }

   gme.designateFamily(
      " name-'product page' y.60 top.40 x.30 x.10 h.30 w.150 font.1 rounding.5 ",
      {"  v.button name.STORE  "
      }
   );
}

CanvasCB cbcg::ServerResponses() {
   gmBuilderHere("ServerResponses  ");
}
// child window 
CanvasCB cbcg::ServerResponses_child() {
   gmBuilderHere("ServerResponses  ");
   gmElements gme;
   // get the info report buffer here
   auto report_lines = net.GetConsoleBuffer();   //

   gme = astr("visual.text name.inforeport font.1 <text> ").append_v(report_lines, "</text>");
}

CanvasCB cbcg::hubmain() {

   gmBuilderHere("LOGOUT  ");

   gmElements gme;
   auto &User = GetUser();

   switch (User.GetHubState()) {
      case hubstate::Create_account: {

         gme.designateFamily(
            " name.cred rounding.20 font.3  x.210 top.90 w.200 h.30 y.100",
            {"v.input input.size.100 name.Username ",
             "v.input input.size.100 name.Password ",
             "v.button name.Create font.2"

            }
         );
         break;
      };

      case hubstate::Login: {
         gme.designateFamily(
            " name.cred rounding.20 font.3  x.210 top.90 w.200 h.30 y.100",
            {"v.input input.size.100 name.Username ",
             "v.input input.size.100 name.Password ",
             "v.button name.Sign-in font.2"

            }
         );
         break;
      };

      case hubstate::Guest: {

         gme = "v.text name.label font.11 <text>PUBLIC_SOURCE_REDACTED</text> x.210";

         gme.designateFamily(
            " name.cred rounding.20 font.1  x.240 top.90 w.150  y.100",
            {"v.button name.Login ",
             "v.button name.Create-Account"

            }
         );
         break;
      };

      case hubstate::Logged: {

         gme = "v.button name.LogOut x.590 font.1 rounding.20 ";
         gme = "  v.text   <text> LOAD GAME NAME_REDACTEDS    </text> font.10 x.10 y.10";

         gme.designateFamily(
            " name='product page' y.60 top.40 x.20 font.2 rounding.5 ",
            {"  v.button name='PUBLIC_SOURCE_REDACTED'  group.store",
             "  v.button name='PUBLIC_SOURCE_REDACTED' group.store",
             "  v.button name='PUBLIC_SOURCE_REDACTED' group.store",
             "  v.button name='PUBLIC_SOURCE_REDACTED' group.store"

            }
         );
      };
   };
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
////
////  @s.server interact here
////
////
////
////

bool net_loadNAME_REDACTED(){

  if(astr pressed_NAME_REDACTED = gm.any_pressed_in_group("store")){
     if(!loader::CoreObject(loader::autoconf::PUBLIC_SOURCE_REDACTED_LOADER).loadProducts_via_proxy(pressed_NAME_REDACTED, false))
        return false;
  }   
  
  return true;
}


bool net_Authorised_SignIn () {


   auto User = GetUser();
   User.GetUserInfo().SetCredentials(gm.getInputBuffer_await("Username"), gm.getInputBuffer_await("Password"));

   if (net.Send(
          "data=",
          {User.GetUserInfo().UserName,
           User.GetUserInfo().Password, User.GetUserInfo().UUID},
          "https://real.PUBLIC_SOURCE_REDACTED.com/routes/login"
       ) != net_ecode::Status_Good) {

      return false;
   }
   else {

      GetUser() = User;
   }


   return true;
}

bool net_OnActivatedKey () {
   auto User = GetUser();
   if (net.Send(
          "data=",
          {User.GetUserInfo().UserName,
           gm.getInputBuffer_await("##key-input"), User.GetUserInfo().UUID
          },
          "https://real.PUBLIC_SOURCE_REDACTED.com/routes/activatekey"
       ) != net_ecode::Status_Good) {

      return false;
   }

   return true;
}

bool net_Account_Created () {
   auto User = GetUser();
   User.GetUserInfo().SetCredentials(gm.getInputBuffer_await("Username"), gm.getInputBuffer_await("Password"));

   if (net.Send(
          "data=",
          {User.GetUserInfo().UserName,
           User.GetUserInfo().Password, User.GetUserInfo().UUID
          },
          "https://real.PUBLIC_SOURCE_REDACTED.com/routes/register"
       ) != net_ecode::Status_Good) {

      return false;
   }
   else {

      GetUser() = User;
   }


   return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
////
////  @s.Runtime Background
////
////  - THIS RUNS IN PARRAREL [AND] BEFORE any gui callbacks
////
////
////  - we should strictly keep GM - non-LOGIC and THAT ONLY here
////
////


auto BackGroundCodeHandler (UserInstance &User) {
   if (net_loadNAME_REDACTED()){
     // set loading screen?
   }
   if (gm.await_pressed("LogOut")) {
      User.SetHubState(hubstate::Guest);
   }

   if (gm.await_pressed("Login")) {
      User.SetHubState(hubstate::Login);
   }

   if (gm.await_pressed("Create-Account")) {
      User.SetHubState(hubstate::Create_account);
   }

   if (gm.await_pressed("Sign-in") and net_Authorised_SignIn()) {
      User.SetHubState(hubstate::Logged);
   }

   if (gm.await_pressed("activate") and net_OnActivatedKey()) {
   }

   if (gm.await_pressed("Create") and net_Account_Created()) {
      User.SetHubState(hubstate::Guest);
   }

   if (gm.await_pressed("STORE"))
      GoToWebStore();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
////
////  @s.entry related code
////
////
////
////


// Function to initialize known user instance
bool initializedKnownUser (UserInstance _ref User, CwService cw = {}) {

   CWUserManager usermanager;

   auto Known = usermanager.DiscoverKnownUser();

   User = Known;

   // initialized known user
   return Known.isKnownUser();
}

bool entry::LoadGuiMaster() {
   try {
      gm.load();
      return true;
   } catch (astr &e) {
      CW_VERBOSE(e);
      return false;
   }
}

bool entry::ResolveServerPackages() {


   return true;
}

noret entry::CCServiceManager() {
   CwService cs;

   cs.cw_UpdateVariableRegistry(
      {"CCNet_isLocalhost", "PUBLIC_SOURCE_REDACTED_path", "curlog", "curpass", "genuName", "LoaderArg", "ProxyArg_type", AccountList_str}
   );

   // shit might be needed right here, loader is so fucking overengineered so idk
}


noret ENTRYPOINT_CW () {
    
gmElements gme;
   
     
   entry::BanDisabler();   //  on compile config


   entry::CCServiceManager();


   entry::ResolveServerPackages();


   if (entry::LoadGuiMaster()) {
      PromptCanvasDesigner();   // GMLANG Canvas init
   }
   else {
      epworker::runtime_error("Unable to initialize guimaster! ");
   }


   UserInstance &User = GetUser();

   if (initializedKnownUser(User))
      CW_VERBOSE(User.GetUserInfo().UserName, "KNOWN USER DISCOVERED");

   User.GetUserInfo().UUID = NetCorp::Getuuid();

   auto MainThreadLoop = [_ref] (auto backgroundCB) {
      while (gm.OnActiveSession()) {
         backgroundCB(User);
         gm.runAll("10ms");
      }
   };

   MainThreadLoop(BackGroundCodeHandler);
}

ep_winapp("REDACTED", ENTRYPOINT_CW, "con");