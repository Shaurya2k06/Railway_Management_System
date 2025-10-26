#include <iostream>

using namespace std;

 

// Custom string copy function

void xcpy_str(char* dest_wp, const char* src_mp) {

   int kj = 0;

   while (src_mp[kj]) {

       dest_wp[kj] = src_mp[kj];

       kj++;

   }

   dest_wp[kj] = '\0';

}

 

// Custom string compare function

int xcmp_str(const char* fx, const char* gy) {

   int pv = 0;

   while (fx[pv] && gy[pv]) {

       if (fx[pv] != gy[pv]) return fx[pv] - gy[pv];

       pv++;

   }

   return fx[pv] - gy[pv];

}

 

// Node for stack to record undoable actions

struct UndoAction_zx {

   int action_type_df;

   char train_identifier_bg[20];

   int previous_delay_qm;

   UndoAction_zx* link_next_pk;

};

 

// Undo stack

class UndoRecordStack_yw {

   UndoAction_zx* stack_top_vh;

public:

   UndoRecordStack_yw() : stack_top_vh(nullptr) {}

   void add_action_tn(int type_code_rs, const char* tid_vb, int old_delay_fn = 0) {

       UndoAction_zx* action_ptr = new UndoAction_zx;

       action_ptr->action_type_df = type_code_rs;

       xcpy_str(action_ptr->train_identifier_bg, tid_vb);

       action_ptr->previous_delay_qm = old_delay_fn;

       action_ptr->link_next_pk = stack_top_vh;

       stack_top_vh = action_ptr;

   }

   UndoAction_zx* remove_action_jc() {

       if (!stack_top_vh) return nullptr;

       UndoAction_zx* removed_item = stack_top_vh;

       stack_top_vh = stack_top_vh->link_next_pk;

       return removed_item;

   }

   bool is_empty_dm() { return !stack_top_vh; }

};

 

// Node for doubly linked list representing a train's schedule

struct ScheduleNode_mp {

   char station_name_wc[20];

   char arrival_time_kz[6];

   ScheduleNode_mp* forward_link_bx;

   ScheduleNode_mp* backward_link_tr;

};

 

class TripSchedule_hq {

   ScheduleNode_mp* list_head_vn;

public:

   TripSchedule_hq() : list_head_vn(nullptr) {}

   void insert_stop_rf(const char* station_id_gm, const char* time_str_pl) {

       ScheduleNode_mp* new_node_sd = new ScheduleNode_mp;

       xcpy_str(new_node_sd->station_name_wc, station_id_gm);

       xcpy_str(new_node_sd->arrival_time_kz, time_str_pl);

       new_node_sd->forward_link_bx = nullptr;

       new_node_sd->backward_link_tr = nullptr;

       

       if (!list_head_vn) {

           list_head_vn = new_node_sd;

       } else {

           ScheduleNode_mp* traverse_ptr = list_head_vn;

           while (traverse_ptr->forward_link_bx) traverse_ptr = traverse_ptr->forward_link_bx;

           traverse_ptr->forward_link_bx = new_node_sd;

           new_node_sd->backward_link_tr = traverse_ptr;

       }

   }

   void display_schedule_qt() {

       ScheduleNode_mp* ptr_walker = list_head_vn;

       cout << "Journey: ";

       while (ptr_walker) {

           cout << "[" << ptr_walker->station_name_wc << "@"

                << ptr_walker->arrival_time_kz << "] ";

           ptr_walker = ptr_walker->forward_link_bx;

       }

       cout << "\n";

   }

};

 

// Hash map for train/passenger data

struct TrainRecord_lw {

   char identifier_code_zr[20];

   int passenger_count_mb;

   int delay_minutes_xh;

   TripSchedule_hq* schedule_ptr_yf;

   bool slot_occupied_np;

};

 

class TrainHashTable_km {

   TrainRecord_lw storage_array_dv[10];

public:

   TrainHashTable_km() {

       for (int slot_idx_fc = 0; slot_idx_fc < 10; ++slot_idx_fc)

           storage_array_dv[slot_idx_fc].slot_occupied_np = false;

   }

   int compute_hash_jt(const char* train_id_wq) {

       int hash_value_bs = 0;

       for (int char_idx_pr = 0; train_id_wq[char_idx_pr]; ++char_idx_pr)

           hash_value_bs = (hash_value_bs * 7 + train_id_wq[char_idx_pr]) % 10;

       return hash_value_bs;

   }

   void store_train_mx(const char* tid_kp, int pax_count_nv, TripSchedule_hq* journey_obj_rh) {

       int slot_position_gf = compute_hash_jt(tid_kp);

       while (storage_array_dv[slot_position_gf].slot_occupied_np &&

              xcmp_str(storage_array_dv[slot_position_gf].identifier_code_zr, tid_kp) != 0)

           slot_position_gf = (slot_position_gf + 1) % 10;

       

       xcpy_str(storage_array_dv[slot_position_gf].identifier_code_zr, tid_kp);

       storage_array_dv[slot_position_gf].slot_occupied_np = true;

       storage_array_dv[slot_position_gf].passenger_count_mb = pax_count_nv;

       storage_array_dv[slot_position_gf].delay_minutes_xh = 0;

       storage_array_dv[slot_position_gf].schedule_ptr_yf = journey_obj_rh;

   }

   TrainRecord_lw* fetch_train_bd(const char* tid_search_qx) {

       int search_pos_lz = compute_hash_jt(tid_search_qx);

       int attempts_counter_vm = 0;

       while (storage_array_dv[search_pos_lz].slot_occupied_np && attempts_counter_vm < 10) {

           if (xcmp_str(storage_array_dv[search_pos_lz].identifier_code_zr, tid_search_qx) == 0)

               return &storage_array_dv[search_pos_lz];

           search_pos_lz = (search_pos_lz + 1) % 10;

           attempts_counter_vm++;

       }

       return nullptr;

   }

   void enumerate_all_ks() {

       for (int array_idx_pt = 0; array_idx_pt < 10; ++array_idx_pt) {

           if (storage_array_dv[array_idx_pt].slot_occupied_np) {

               cout << "- ID: " << storage_array_dv[array_idx_pt].identifier_code_zr

                    << " (" << storage_array_dv[array_idx_pt].passenger_count_mb

                    << " pax, " << storage_array_dv[array_idx_pt].delay_minutes_xh

                    << " min delay)\n";

           }

       }

   }

};

 

// Graph: adjacency matrix for station connections

class StationNetwork_cw {

   char station_labels_nt[6][20];

   int connection_matrix_fh[6][6];

public:

   StationNetwork_cw() {

       xcpy_str(station_labels_nt[0], "Central");

       xcpy_str(station_labels_nt[1], "North");

       xcpy_str(station_labels_nt[2], "South");

       xcpy_str(station_labels_nt[3], "East");

       xcpy_str(station_labels_nt[4], "West");

       xcpy_str(station_labels_nt[5], "Yard");

       

       for (int row_idx_bm = 0; row_idx_bm < 6; ++row_idx_bm)

           for (int col_idx_sv = 0; col_idx_sv < 6; ++col_idx_sv)

               connection_matrix_fh[row_idx_bm][col_idx_sv] = 0;

       

       connection_matrix_fh[0][1] = connection_matrix_fh[1][0] = 1;

       connection_matrix_fh[0][2] = connection_matrix_fh[2][0] = 1;

       connection_matrix_fh[0][3] = connection_matrix_fh[3][0] = 1;

       connection_matrix_fh[0][4] = connection_matrix_fh[4][0] = 1;

       connection_matrix_fh[2][5] = connection_matrix_fh[5][2] = 1;

   }

   void display_network_hz() {

       cout << "Station Connections:\n";

       for (int outer_loop_qd = 0; outer_loop_qd < 6; ++outer_loop_qd) {

           cout << "  " << station_labels_nt[outer_loop_qd] << ":";

           for (int inner_loop_wk = 0; inner_loop_wk < 6; ++inner_loop_wk)

               if (connection_matrix_fh[outer_loop_qd][inner_loop_wk]) {

                   cout << " " << station_labels_nt[inner_loop_wk];

               }

           cout << "\n";

       }

   }

};

 

// Min-heap for reschedule queue

struct QueueElement_jp {

   int priority_value_xn;

   char train_code_hv[20];

};

 

class UrgencyHeap_bg {

   QueueElement_jp heap_storage_mt[10];

   int element_count_zd;

public:

   UrgencyHeap_bg() : element_count_zd(0) {}

   void add_element_vr(int urgency_param_lk, const char* id_str_pw) {

       heap_storage_mt[element_count_zd].priority_value_xn = urgency_param_lk;

       xcpy_str(heap_storage_mt[element_count_zd].train_code_hv, id_str_pw);

       int bubble_idx_tq = element_count_zd++;

       

       while (bubble_idx_tq > 0 &&

              heap_storage_mt[(bubble_idx_tq - 1) / 2].priority_value_xn >

              heap_storage_mt[bubble_idx_tq].priority_value_xn) {

           QueueElement_jp swap_temp_nk = heap_storage_mt[bubble_idx_tq];

           heap_storage_mt[bubble_idx_tq] = heap_storage_mt[(bubble_idx_tq - 1) / 2];

           heap_storage_mt[(bubble_idx_tq - 1) / 2] = swap_temp_nk;

           bubble_idx_tq = (bubble_idx_tq - 1) / 2;

       }

   }

   QueueElement_jp extract_min_fs() {

       QueueElement_jp root_element_cr = heap_storage_mt[0];

       heap_storage_mt[0] = heap_storage_mt[--element_count_zd];

       restore_heap_prop_dw(0);

       return root_element_cr;

   }

   bool check_empty_rx() { return element_count_zd == 0; }

   void restore_heap_prop_dw(int node_idx_zm) {

       int left_child_bk = 2 * node_idx_zm + 1;

       int right_child_yn = 2 * node_idx_zm + 2;

       int smallest_idx_pf = node_idx_zm;

       

       if (left_child_bk < element_count_zd &&

           heap_storage_mt[left_child_bk].priority_value_xn <

           heap_storage_mt[smallest_idx_pf].priority_value_xn)

           smallest_idx_pf = left_child_bk;

       

       if (right_child_yn < element_count_zd &&

           heap_storage_mt[right_child_yn].priority_value_xn <

           heap_storage_mt[smallest_idx_pf].priority_value_xn)

           smallest_idx_pf = right_child_yn;

       

       if (smallest_idx_pf != node_idx_zm) {

           QueueElement_jp swap_buffer_vh = heap_storage_mt[node_idx_zm];

           heap_storage_mt[node_idx_zm] = heap_storage_mt[smallest_idx_pf];

           heap_storage_mt[smallest_idx_pf] = swap_buffer_vh;

           restore_heap_prop_dw(smallest_idx_pf);

       }

   }

   void display_queue_content_mj() {

       cout << "Reschedule Queue:\n";

       for (int display_idx_hr = 0; display_idx_hr < element_count_zd; ++display_idx_hr) {

           cout << heap_storage_mt[display_idx_hr].train_code_hv

                << ": urgency=" << heap_storage_mt[display_idx_hr].priority_value_xn << "\n";

       }

   }

};

 

// Main admin menu loop

int main() {

   TrainHashTable_km train_repository_xc;

   UrgencyHeap_bg priority_manager_kf;

   UndoRecordStack_yw undo_history_qz;

   StationNetwork_cw network_topology_sj;

 

   while (true) {

       cout << "\nTrain Admin CLI Menu:\n"

            << "1. Schedule Train\n"

            << "2. List All Trains\n"

            << "3. Report Delay\n"

            << "4. Reschedule Next Train\n"

            << "5. Print Priority Queue\n"

            << "6. Journey Edit\n"

            << "7. Undo Last Action\n"

            << "8. Show Network Map\n"

            << "9. Exit\n";

       

       int user_choice_mb;

       cin >> user_choice_mb;

       

       if (user_choice_mb == 1) {

           char train_id_buffer_gx[20];

           int passenger_num_tk, stop_count_vd;

           cout << "Train ID: ";

           cin >> train_id_buffer_gx;

           cout << "# Passengers: ";

           cin >> passenger_num_tk;

           cout << "# Journey stops: ";

           cin >> stop_count_vd;

           

           TripSchedule_hq* journey_plan_bf = new TripSchedule_hq();

           for (int stop_iter_nq = 0; stop_iter_nq < stop_count_vd; ++stop_iter_nq) {

               char station_buf_rv[20], time_buf_jm[6];

               cout << "  Stop " << (stop_iter_nq + 1) << " station: ";

               cin >> station_buf_rv;

               cout << "  Arrival time (hh:mm): ";

               cin >> time_buf_jm;

               journey_plan_bf->insert_stop_rf(station_buf_rv, time_buf_jm);

           }

           train_repository_xc.store_train_mx(train_id_buffer_gx, passenger_num_tk, journey_plan_bf);

           cout << "Train scheduled.\n";

       }

       else if (user_choice_mb == 2) {

           train_repository_xc.enumerate_all_ks();

       }

       else if (user_choice_mb == 3) {

           char delay_train_id_cz[20];

           int delay_amount_pr;

           cout << "Which train? ";

           cin >> delay_train_id_cz;

           cout << "Delay minutes: ";

           cin >> delay_amount_pr;

           

           TrainRecord_lw* train_ptr_wh = train_repository_xc.fetch_train_bd(delay_train_id_cz);

           if (train_ptr_wh) {

               int urgency_calc_dm = delay_amount_pr * (1000 / (train_ptr_wh->passenger_count_mb + 1));

               priority_manager_kf.add_element_vr(urgency_calc_dm, delay_train_id_cz);

               undo_history_qz.add_action_tn(1, delay_train_id_cz, train_ptr_wh->delay_minutes_xh);

               train_ptr_wh->delay_minutes_xh += delay_amount_pr;

               cout << "Delay posted.\n";

           } else {

               cout << "Train not found.\n";

           }

       }

       else if (user_choice_mb == 4) {

           if (!priority_manager_kf.check_empty_rx()) {

               QueueElement_jp extracted_entry_fk = priority_manager_kf.extract_min_fs();

               cout << "Rescheduling: " << extracted_entry_fk.train_code_hv

                    << " (urgency " << extracted_entry_fk.priority_value_xn << ")\n";

               undo_history_qz.add_action_tn(2, extracted_entry_fk.train_code_hv, 0);

           } else {

               cout << "No train to reschedule.\n";

           }

       }

       else if (user_choice_mb == 5) {

           priority_manager_kf.display_queue_content_mj();

       }

       else if (user_choice_mb == 6) {

           char journey_train_id_ql[20];

           cout << "Train ID: ";

           cin >> journey_train_id_ql;

           TrainRecord_lw* journey_train_ptr_xb = train_repository_xc.fetch_train_bd(journey_train_id_ql);

           if (journey_train_ptr_xb && journey_train_ptr_xb->schedule_ptr_yf)

               journey_train_ptr_xb->schedule_ptr_yf->display_schedule_qt();

           else

               cout << "Not found.\n";

       }

       else if (user_choice_mb == 7) {

           if (!undo_history_qz.is_empty_dm()) {

               UndoAction_zx* undo_item_tp = undo_history_qz.remove_action_jc();

               cout << "Undo type " << undo_item_tp->action_type_df

                    << " for train " << undo_item_tp->train_identifier_bg << "\n";

               delete undo_item_tp;

           } else {

               cout << "Nothing to undo.\n";

           }

       }

       else if (user_choice_mb == 8) {

           network_topology_sj.display_network_hz();

       }

       else if (user_choice_mb == 9) {

           cout << "Bye.\n";

           break;

       }

   }

   return 0;

}