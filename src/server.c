/**
 *******************************************************************************
 * @file       server.c
 * @version    V0.7.4
 * @date       2020.04.18
 * @brief      The server for gui engine.	
 *******************************************************************************
 */ 

#include <cogui.h>

app_t *server_app = Co_NULL;
OS_STK   server_Stk[512]={0};
extern window_t *main_page;
extern struct main_app_table main_app_table[9];

void gui_server_handler_mouse_button(event_t *event)
{
    gui_mouse_return_picture();
    if (gui_get_current_window() == gui_get_main_window()) {
        point_t cursor_pt;
        widget_t *event_wgt, *last_ewgt = main_page->last_mouse_event_widget;
        gui_mouse_get_position(&cursor_pt);
        event_wgt = gui_window_get_mouse_event_widget(gui_app_self()->win, cursor_pt.x, cursor_pt.y);

        if (event_wgt && event_wgt->user_data) {
            if ((event->button & MOUSE_BUTTON_UP) && last_ewgt == event_wgt) {
                app_t *eapp = (app_t *)event_wgt->user_data;
                EVENT_INIT(event, EVENT_PAINT);
                event->app = eapp;
            }
        } else {
            gui_mouse_show();
            return;
        }
    } else {
        EVENT_INIT(event, EVENT_MOUSE_BUTTON);
        event->app = gui_get_current_window()->app;
    }

    if (event->app != Co_NULL) {
        while(gui_send(event->app, event) != GUI_E_OK){
            CoTickDelay(50);
        }
    }
}

void gui_server_handler_mouse_motion(event_t *event)
{
    gui_mouse_move_delta(event->dx, event->dy);
}

void gui_server_event_kbd(event_t *event)
{
    event->ascii_code = 0;      /* initial event ascii code */
    if (event->kbd_type == KBD_KEYDOWN) { /* parse ascii code if need  */
        if (event->key >= KBD_KEY_SPACE && event->key <= KBD_KEY_TILDE) {
            event->ascii_code = event->key;
            if (KBD_IS_CAPS_LOCK(event)) {
                if (event->key >= KBD_KEY_LOWER_A && event->key <= KBD_KEY_LOWER_Z) {
                    event->ascii_code = event->key - 32;
                } else if (event->key >= KBD_KEY_UPPER_A && event->key <= KBD_KEY_UPPER_Z) {
                    event->ascii_code = event->key + 32;
                }
            }
        } else if (event->key == KBD_KEY_ENTER || event->key == KBD_KEY_TAB) {
            event->ascii_code = event->key;
        } else if (event->key >= KBD_KEY_NUM_0 && event->key <= KBD_KEY_NUM_ENTER) {
            if (KBD_IS_KP(event)) {
                const int8_t kp_keys[] = "0123456789./*-+\n";
                event->ascii_code = kp_keys[event->key - KBD_KEY_NUM_0];
            }
        }
    }

    if (gui_get_current_window() == gui_get_main_window()) {
        /* also handle some shortcuts */
        if (event->ascii_code >= '1' && event->ascii_code <= '9') {
            widget_t *event_wgt = main_app_table[event->ascii_code - '1'].app_icon;    
            app_t *eapp = (app_t *)event_wgt->user_data;
            if (eapp) {
                EVENT_INIT(event, EVENT_PAINT);
                event->app = eapp;
            }
        } else {
            event->app = Co_NULL;
        }
    } else {
        EVENT_INIT(event, EVENT_KBD);
        event->app = gui_get_current_window()->app;
    }

          
    if (event->app != Co_NULL) {
        gui_send(event->app, event);
    }  
}

StatusType gui_server_event_handler(event_t *event)
{
    ASSERT(event != Co_NULL);

    StatusType result = GUI_E_ERROR;


    switch (event->type)
    {
	case EVENT_APP_CREATE:
    case EVENT_APP_DELE:
		result = gui_ack(event, E_OK);
		break;

    case EVENT_PAINT:
        //result = gui_send(event->sender, event);
        break;
		
    /* mouse and keyboard event */
    case EVENT_MOUSE_BUTTON:
        gui_server_handler_mouse_button(event);
        break;

    case EVENT_MOUSE_MOTION:
        gui_server_handler_mouse_motion(event);
        break;

    case EVENT_KBD:
        gui_server_event_kbd(event);
        break;

    case EVENT_WINDOW_CLOSE:
    {
        if (event->win) {
            result = GUI_E_ERROR;
        }

        result = gui_window_show(server_app->win);    
        gui_mouse_show();
        break;
    }

    case EVENT_WINDOW_HIDE:
    {
        if (GUI_WINDOW_IS_ENABLE(event->win)) {
            result = GUI_E_ERROR;
        }

        result = gui_window_show(server_app->win);    
        gui_mouse_show();
        break;
    }
        
    default:
        break;
    }

    return result;
}

void gui_server_entry(void *parameter)
{
    server_app = gui_app_create("Server");
    if(server_app == Co_NULL){
        return;
    }

    server_app->handler = gui_server_event_handler;

    gui_app_run(server_app);
    gui_app_delete(server_app);
    server_app = Co_NULL;

    CoExitTask();
}

StatusType gui_server_post_event(event_t *event)
{
    StatusType result;

    if (server_app != Co_NULL){
        result = gui_send(server_app, event);
    }
    else{
        result = GUI_E_ERROR;
    }

    return result;
}

StatusType gui_server_post_event_sync(event_t *event)
{
    StatusType result;

    if (server_app != Co_NULL){
        result = gui_send_sync(server_app, event);
    }
    else{
        result = GUI_E_ERROR;
    }

    return result;
}


app_t *gui_get_server(void)
{
    return server_app;
}

void gui_server_init(void)
{
    CoCreateTask(gui_server_entry, (void *)0, 15,&server_Stk[511], 512);
}
