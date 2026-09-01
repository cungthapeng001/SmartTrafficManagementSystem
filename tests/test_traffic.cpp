#include <iostream>
#include <cassert>
#include "TrafficManagementSystem.h"

using namespace std;

void runTests() {
    cout << "Running tests...\n" << endl;

    TrafficManagementSystem tms;

    // =========================================================================
    // စမ်းသပ်မှု ၁: အလွတ်ဖြစ်နေသော queue ဖြင့် စတင်ခြင်း
    // 
    // Data Structure သဘောတရား: Queue စတင်ခြင်းနှင့် Underflow ကို ကိုင်တွယ်ဖြေရှင်းခြင်း
    // ရှင်းလင်းချက်: Queue သည် သီးခြားအစီအစဉ်တစ်ခုဖြင့် element များကို သိမ်းဆည်းပေးသော စိတ္တဇ data structure တစ်ခုဖြစ်ပါသည်။ 
    // Queue ကို စတင်ဖန်တီးသောအခါ ၎င်းတွင် element တစ်ခုမှ မရှိပါ။ အလွတ်ဖြစ်နေသော Queue မှ element တစ်ခုကို ဖယ်ရှားရန် 
    // ကြိုးစားပါက (Dequeue) "Underflow" ဟုခေါ်သော အခြေအနေတစ်ခု ဖြစ်ပေါ်လာသည်။ ကျွန်ုပ်တို့၏စနစ်သည် ၎င်းကို မပျက်စီးစေဘဲ 
    // ကောင်းမွန်စွာ ကိုင်တွယ်ဖြေရှင်းရမည်။
    // =========================================================================
    cout << "Test 1: Empty Queue" << endl;
    assert(tms.getVehicleCount() == 0);
    Vehicle dummy("", "", "");
    assert(tms.removeVehicle(dummy) == false); // မပျက်စီးသင့်ပါ၊ false ကို ပြန်ပေးသည်
    cout << "Test 1 Passed!\n" << endl;

    // =========================================================================
    // စမ်းသပ်မှု ၂: ယာဉ် ၁ စီး ထည့်ခြင်း
    // 
    // Data Structure သဘောတရား: Enqueue လုပ်ဆောင်ချက်
    // ရှင်းလင်းချက်: Queue ထဲသို့ element တစ်ခုထည့်ခြင်းကို "Enqueue" ဟုခေါ်သည်။ C++ STL တွင် 
    // ၎င်းကို push() function ကို အသုံးပြု၍ ပြုလုပ်ပြီး ၎င်းသည် element ကို queue ၏ "အနောက်" (သို့) "အမြီး" တွင် 
    // ထည့်သွင်းပေးပါသည်။ ထို့အပြင် ယာဉ် ၁ စီးသည် "LOW" (အနည်း) ယာဉ်ကြောကျပ်တည်းမှု စည်းမျဉ်း (ယာဉ် ၀-၅ စီး) ကို 
    // ဖြစ်ပေါ်စေပြီး ၃၀ စက္ကန့် စိမ်းရောင်မီးကို သတ်မှတ်ပေးကြောင်း ကျွန်ုပ်တို့ စစ်ဆေးပါသည်။
    // =========================================================================
    cout << "Test 2: Add 1 vehicle (LOW density)" << endl;
    tms.addVehicle(Vehicle("V1", "Car", "Main St")); // V1 ကို Enqueue လုပ်သည်
    assert(tms.getVehicleCount() == 1);
    assert(tms.getTrafficDensity() == "LOW");
    assert(tms.getGreenSignalTime() == 30);
    cout << "Test 2 Passed!\n" << endl;

    // =========================================================================
    // စမ်းသပ်မှု ၃: နောက်ထပ် ယာဉ် ၅ စီး ထည့်ခြင်း (စုစုပေါင်း ၆ စီး -> MEDIUM အလယ်အလတ် ကျပ်တည်းမှု)
    // 
    // Data Structure သဘောတရား: အစဉ်လိုက် Enqueue ပြုလုပ်ခြင်း
    // ရှင်းလင်းချက်: ကျွန်ုပ်တို့သည် element များကို ဆက်တိုက်ထည့်သွင်းပါသည်။ ၎င်းသည် Queue ဖြစ်သောကြောင့် 
    // V2 သည် V1 နောက်သို့၊ V3 သည် V2 နောက်သို့ ရောက်သွားပါသည်။ ၎င်းသည် မီးနီတွင် တကယ့် ယာဉ်များတန်းစီနေခြင်းကို 
    // ပုံဖော်ထားခြင်းဖြစ်ပါသည်။ ယာဉ် ၆ စီးသည် ယာဉ်ကြောကျပ်တည်းမှုကို "MEDIUM" (ယာဉ် ၆-၁၀ စီး) သို့ 
    // ပြောင်းလဲစေပြီး ၆၀ စက္ကန့် စိမ်းရောင်မီးကို သတ်မှတ်ပေးကြောင်းကိုလည်း စစ်ဆေးပါသည်။
    // =========================================================================
    cout << "Test 3: Add up to 6 vehicles (MEDIUM density)" << endl;
    tms.addVehicle(Vehicle("V2", "Bike", "Main St"));
    tms.addVehicle(Vehicle("V3", "Car", "Main St"));
    tms.addVehicle(Vehicle("V4", "Bus", "Main St"));
    tms.addVehicle(Vehicle("V5", "Car", "Main St"));
    tms.addVehicle(Vehicle("V6", "Car", "Main St"));
    
    assert(tms.getVehicleCount() == 6);
    assert(tms.getTrafficDensity() == "MEDIUM");
    assert(tms.getGreenSignalTime() == 60);
    cout << "Test 3 Passed!\n" << endl;

    // =========================================================================
    // စမ်းသပ်မှု ၄: နောက်ထပ် ယာဉ် ၅ စီး ထည့်ခြင်း (စုစုပေါင်း ၁၁ စီး -> HIGH အများဆုံး ကျပ်တည်းမှု)
    // 
    // သဘောတရား စစ်ဆေးခြင်း: တက်ကြွသော စည်းမျဉ်းများ အသုံးချခြင်း
    // ရှင်းလင်းချက်: ယခု Queue အရွယ်အစားမှာ ၁၁ ဖြစ်သည်။ ၎င်းသည် ယာဉ် ၁၀ စီး ကန့်သတ်ချက်ကို ကျော်လွန်သွားပြီး 
    // စနစ်ကို ၉၀ စက္ကန့် စိမ်းရောင်မီးဖြင့် "HIGH" ကျပ်တည်းမှု အုပ်စုထဲသို့ ရောက်သွားစေပါသည်။
    // =========================================================================
    cout << "Test 4: Add up to 11 vehicles (HIGH density)" << endl;
    tms.addVehicle(Vehicle("V7", "Car", "Main St"));
    tms.addVehicle(Vehicle("V8", "Car", "Main St"));
    tms.addVehicle(Vehicle("V9", "Truck", "Main St"));
    tms.addVehicle(Vehicle("V10", "Car", "Main St"));
    tms.addVehicle(Vehicle("V11", "Bike", "Main St"));

    assert(tms.getVehicleCount() == 11);
    assert(tms.getTrafficDensity() == "HIGH");
    assert(tms.getGreenSignalTime() == 90);
    cout << "Test 4 Passed!\n" << endl;

    // =========================================================================
    // စမ်းသပ်မှု ၆: Queue ကို ပြသခြင်း (Queue ကို မပြောင်းလဲစေရပါ)
    // 
    // Data Structure သဘောတရား: မပျက်စီးစေသော ဖြတ်သန်းသွားလာခြင်း
    // ရှင်းလင်းချက်: ပုံမှန် Queue တစ်ခုသည် "ရှေ့ဆုံး" (front) element ကိုသာ ရယူခွင့်ပြုပါသည်။ Array 
    // သို့မဟုတ် Linked List ကဲ့သို့ ၎င်းကို တစ်ခုချင်းစီ ဖြတ်သန်း၍မရပါ။ Element အားလုံးကို ပြသရန်အတွက် ကျွန်ုပ်တို့သည် 
    // Queue ၏ *မိတ္တူ* (copy) တစ်ခုကို ဖန်တီးရမည်ဖြစ်ပြီး မိတ္တူမှ element များကို dequeue လုပ်၍ print ထုတ်ရမည်ဖြစ်ကာ 
    // မူရင်း Queue ကို မူလအတိုင်း ထားရှိရပါမည်။
    // =========================================================================
    cout << "Test 6: Display Queue (Non-destructive)" << endl;
    tms.displayQueue(); // မျက်စိဖြင့် ကြည့်ရှုစစ်ဆေးပါ
    assert(tms.getVehicleCount() == 11); // အရွယ်အစား မပြောင်းလဲကြောင်း သေချာစေပါ
    cout << "Test 6 Passed!\n" << endl;

    // =========================================================================
    // စမ်းသပ်မှု ၅: ယာဉ်များကို ဖယ်ရှားခြင်း (FIFO အစီအစဉ်)
    // 
    // Data Structure သဘောတရား: Dequeue နှင့် FIFO မူအရ
    // ရှင်းလင်းချက်: FIFO ဆိုသည်မှာ ပထမဆုံးဝင်လာသူ ပထမဆုံးထွက်ရမည် (First-In, First-Out) ကို ဆိုလိုပါသည်။ 
    // Queue ထဲသို့ ပထမဆုံးထည့်ထားသော ယာဉ် (V1) သည် မီးစိမ်းလာသောအခါ ပထမဆုံး ထွက်ခွာရမည့် ယာဉ်ဖြစ်ရပါမည်။ 
    // ၎င်းကို front() element ကို ကြည့်ရှုပြီး ၎င်းကို ဖယ်ရှားရန် pop() ကို အသုံးပြုခြင်းဖြင့် ပြုလုပ်ပါသည်။ 
    // ဤနေရာတွင် ဖယ်ရှားလိုက်သော ယာဉ်သည် တကယ်ပင် "V1" ဖြစ်ကြောင်း ကျွန်ုပ်တို့ စစ်ဆေးပါသည်။
    // =========================================================================
    cout << "Test 5: Remove Vehicles (FIFO)" << endl;
    Vehicle removed("", "", "");
    bool success = tms.removeVehicle(removed); // Dequeue လုပ်ဆောင်ချက်
    assert(success == true);
    assert(removed.getVehicleID() == "V1"); // ပထမဆုံးဝင်လာသူ ပထမဆုံးထွက်ကြောင်း သက်သေ
    assert(tms.getVehicleCount() == 10); // Queue သည် ၁ ခု လျော့သွားသည်
    cout << "Test 5 Passed!\n" << endl;

    cout << "All tests passed successfully!" << endl;
}

int main() {
    runTests();
    return 0;
}
