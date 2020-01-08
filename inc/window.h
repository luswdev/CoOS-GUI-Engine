/**
 *******************************************************************************
 * @file       window.h
 * @version    V0.0.3 
 * @date       2020.01.04
 * @brief      Window management function header file.
 *******************************************************************************
 */ 

#ifndef _COGUI_WINDOW_H
#define _COGUI_WINDOW_H

/* window style */
#define COGUI_WINDOW_STYLE_NO_TITLE            0x01  /**< no title window               */
#define COGUI_WINDOW_STYLE_NO_BORDER           0x02  /**< no border window              */

#define COGUI_WINDOW_MAGIC					  0x57696E00		/* win magic flag */

/* window flag */
#define COGUI_WINDOW_FLAG_INIT        0x00
#define COGUI_WINDOW_FLAG_SHOW        0x01

#define COGUI_WINDOW(w)     ((struct cogui_window *)(w))

/* enable (show) */
#define COGUI_WINDOW_ENABLE(w)          COGUI_WINDOW((w))->flag |= COGUI_WINDOW_FLAG_SHOW
#define COGUI_WINDOW_DISABLE(w)         COGUI_WINDOW((w))->flag &= ~COGUI_WINDOW_FLAG_SHOW
#define COGUI_WINDOW_IS_ENABLE(w)       (COGUI_WINDOW((w))->flag & COGUI_WINDOW_FLAG_SHOW)

#define cogui_window_create_with_title()        cogui_window_create(~COGUI_WINDOW_STYLE_NO_TITLE)
#define cogui_window_create_without_title()     cogui_window_create(COGUI_WINDOW_STYLE_NO_TITLE)

struct cogui_window
{
    /* window magic word  field */
    co_uint32_t         magic;                          /**< should be 0x57696E00 */

    /* meta data field */
    cogui_widget_t *    widget_list;                    /**<*/
    co_int16_t          id;                             /**<*/
    co_uint16_t         style;                          /**<*/
    co_int32_t          flag;                           /**<*/
    co_int64_t          update;                         /**<*/
    co_int32_t          widget_cnt;                     /**<*/
    cogui_widget_t *    focus_widget;                   /**<*/
    cogui_app_t *       app;                            /**<*/
    cogui_widget_t *    title;                          /**<*/
    char *              title_name;                     /**<*/

    /* event pointer feild */
    cogui_widget_t *    last_mouse_event_widget;        /**<*/

    /* user private data field */
    void *              user_data;                      /**<*/

    /* event handler field */
    StatusType (*on_activate)(cogui_widget_t *widget, struct cogui_event *event);   /**<*/
    StatusType (*on_deactivate)(cogui_widget_t *widget, struct cogui_event *event); /**<*/
    StatusType (*on_key)(cogui_widget_t *widget, struct cogui_event *event);        /**<*/
    co_uint64_t (*_do_show)(struct cogui_window *win);                              /**<*/
    StatusType  (*handler)(struct cogui_window *widget ,struct cogui_event *event); /**<*/
};
typedef struct cogui_window cogui_window_t;

struct main_app_table {
    cogui_widget_t *app_icon;
    cogui_widget_t *app_title_box;
    char *          title;    
};

cogui_window_t *cogui_window_create(co_uint16_t style);
void cogui_window_delete(cogui_window_t *win);

cogui_window_t *cogui_main_window_create(void);

cogui_window_t *cogui_get_current_window(void);

StatusType cogui_window_close(cogui_window_t *win);

StatusType cogui_window_show(cogui_window_t *win);
StatusType cogui_window_onshow(cogui_window_t *win);

StatusType cogui_window_hide(cogui_window_t *win);
StatusType cogui_window_onhide(cogui_window_t *win);

StatusType cogui_window_active(cogui_window_t *win);
StatusType cogui_window_is_activated(cogui_window_t *win);

void cogui_window_set_onactivate(cogui_window_t *win, event_handler_ptr handler);
void cogui_window_set_ondeactive(cogui_window_t *win, event_handler_ptr handler);
void cogui_window_set_onkey(cogui_window_t *win, event_handler_ptr handler);

void cogui_window_set_title(cogui_window_t *win, const U8 *title);
co_uint8_t *cogui_window_get_title(cogui_window_t *win);

void cogui_assert_failed_page(const char* ex, co_uint16_t line, const char* func);

#endif /* _COGUI_WINDOW_H */