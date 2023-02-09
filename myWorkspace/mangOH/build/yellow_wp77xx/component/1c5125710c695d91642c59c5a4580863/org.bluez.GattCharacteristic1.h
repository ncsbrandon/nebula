/*
 * Generated by gdbus-codegen 2.54.3. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef __ORG_BLUEZ_GATTCHARACTERISTIC1_H__
#define __ORG_BLUEZ_GATTCHARACTERISTIC1_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.bluez.GattCharacteristic1 */

#define BLUEZ_TYPE_GATT_CHARACTERISTIC1 (bluez_gatt_characteristic1_get_type ())
#define BLUEZ_GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1, BluezGattCharacteristic1))
#define BLUEZ_IS_GATT_CHARACTERISTIC1(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1))
#define BLUEZ_GATT_CHARACTERISTIC1_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1, BluezGattCharacteristic1Iface))

struct _BluezGattCharacteristic1;
typedef struct _BluezGattCharacteristic1 BluezGattCharacteristic1;
typedef struct _BluezGattCharacteristic1Iface BluezGattCharacteristic1Iface;

struct _BluezGattCharacteristic1Iface
{
  GTypeInterface parent_iface;


  gboolean (*handle_acquire_notify) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_options);

  gboolean (*handle_acquire_write) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_options);

  gboolean (*handle_confirm) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_read_value) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_options);

  gboolean (*handle_start_notify) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_stop_notify) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_write_value) (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *arg_value,
    GVariant *arg_options);

  const gchar *const * (*get_flags) (BluezGattCharacteristic1 *object);

  guint16  (*get_handle) (BluezGattCharacteristic1 *object);

  gboolean  (*get_notify_acquired) (BluezGattCharacteristic1 *object);

  gboolean  (*get_notifying) (BluezGattCharacteristic1 *object);

  const gchar * (*get_service) (BluezGattCharacteristic1 *object);

  const gchar * (*get_uuid) (BluezGattCharacteristic1 *object);

  GVariant * (*get_value) (BluezGattCharacteristic1 *object);

  gboolean  (*get_write_acquired) (BluezGattCharacteristic1 *object);

};

GType bluez_gatt_characteristic1_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *bluez_gatt_characteristic1_interface_info (void);
guint bluez_gatt_characteristic1_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void bluez_gatt_characteristic1_complete_read_value (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *value);

void bluez_gatt_characteristic1_complete_write_value (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void bluez_gatt_characteristic1_complete_acquire_write (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *fd,
    guint16 mtu);

void bluez_gatt_characteristic1_complete_acquire_notify (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation,
    GVariant *fd,
    guint16 mtu);

void bluez_gatt_characteristic1_complete_start_notify (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void bluez_gatt_characteristic1_complete_stop_notify (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);

void bluez_gatt_characteristic1_complete_confirm (
    BluezGattCharacteristic1 *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void bluez_gatt_characteristic1_call_read_value (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_read_value_finish (
    BluezGattCharacteristic1 *proxy,
    GVariant **out_value,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_read_value_sync (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GVariant **out_value,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_write_value (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_value,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_write_value_finish (
    BluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_write_value_sync (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_value,
    GVariant *arg_options,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_acquire_write (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_acquire_write_finish (
    BluezGattCharacteristic1 *proxy,
    GVariant **out_fd,
    guint16 *out_mtu,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_acquire_write_sync (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GVariant **out_fd,
    guint16 *out_mtu,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_acquire_notify (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_acquire_notify_finish (
    BluezGattCharacteristic1 *proxy,
    GVariant **out_fd,
    guint16 *out_mtu,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_acquire_notify_sync (
    BluezGattCharacteristic1 *proxy,
    GVariant *arg_options,
    GVariant **out_fd,
    guint16 *out_mtu,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_start_notify (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_start_notify_finish (
    BluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_start_notify_sync (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_stop_notify (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_stop_notify_finish (
    BluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_stop_notify_sync (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);

void bluez_gatt_characteristic1_call_confirm (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean bluez_gatt_characteristic1_call_confirm_finish (
    BluezGattCharacteristic1 *proxy,
    GAsyncResult *res,
    GError **error);

gboolean bluez_gatt_characteristic1_call_confirm_sync (
    BluezGattCharacteristic1 *proxy,
    GCancellable *cancellable,
    GError **error);



/* D-Bus property accessors: */
const gchar *bluez_gatt_characteristic1_get_uuid (BluezGattCharacteristic1 *object);
gchar *bluez_gatt_characteristic1_dup_uuid (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_uuid (BluezGattCharacteristic1 *object, const gchar *value);

const gchar *bluez_gatt_characteristic1_get_service (BluezGattCharacteristic1 *object);
gchar *bluez_gatt_characteristic1_dup_service (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_service (BluezGattCharacteristic1 *object, const gchar *value);

GVariant *bluez_gatt_characteristic1_get_value (BluezGattCharacteristic1 *object);
GVariant *bluez_gatt_characteristic1_dup_value (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_value (BluezGattCharacteristic1 *object, GVariant *value);

gboolean bluez_gatt_characteristic1_get_write_acquired (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_write_acquired (BluezGattCharacteristic1 *object, gboolean value);

gboolean bluez_gatt_characteristic1_get_notify_acquired (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_notify_acquired (BluezGattCharacteristic1 *object, gboolean value);

gboolean bluez_gatt_characteristic1_get_notifying (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_notifying (BluezGattCharacteristic1 *object, gboolean value);

const gchar *const *bluez_gatt_characteristic1_get_flags (BluezGattCharacteristic1 *object);
gchar **bluez_gatt_characteristic1_dup_flags (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_flags (BluezGattCharacteristic1 *object, const gchar *const *value);

guint16 bluez_gatt_characteristic1_get_handle (BluezGattCharacteristic1 *object);
void bluez_gatt_characteristic1_set_handle (BluezGattCharacteristic1 *object, guint16 value);


/* ---- */

#define BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY (bluez_gatt_characteristic1_proxy_get_type ())
#define BLUEZ_GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY, BluezGattCharacteristic1Proxy))
#define BLUEZ_GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY, BluezGattCharacteristic1ProxyClass))
#define BLUEZ_GATT_CHARACTERISTIC1_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY, BluezGattCharacteristic1ProxyClass))
#define BLUEZ_IS_GATT_CHARACTERISTIC1_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY))
#define BLUEZ_IS_GATT_CHARACTERISTIC1_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), BLUEZ_TYPE_GATT_CHARACTERISTIC1_PROXY))

typedef struct _BluezGattCharacteristic1Proxy BluezGattCharacteristic1Proxy;
typedef struct _BluezGattCharacteristic1ProxyClass BluezGattCharacteristic1ProxyClass;
typedef struct _BluezGattCharacteristic1ProxyPrivate BluezGattCharacteristic1ProxyPrivate;

struct _BluezGattCharacteristic1Proxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  BluezGattCharacteristic1ProxyPrivate *priv;
};

struct _BluezGattCharacteristic1ProxyClass
{
  GDBusProxyClass parent_class;
};

GType bluez_gatt_characteristic1_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (BluezGattCharacteristic1Proxy, g_object_unref)
#endif

void bluez_gatt_characteristic1_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
BluezGattCharacteristic1 *bluez_gatt_characteristic1_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
BluezGattCharacteristic1 *bluez_gatt_characteristic1_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void bluez_gatt_characteristic1_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
BluezGattCharacteristic1 *bluez_gatt_characteristic1_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
BluezGattCharacteristic1 *bluez_gatt_characteristic1_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON (bluez_gatt_characteristic1_skeleton_get_type ())
#define BLUEZ_GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON, BluezGattCharacteristic1Skeleton))
#define BLUEZ_GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON, BluezGattCharacteristic1SkeletonClass))
#define BLUEZ_GATT_CHARACTERISTIC1_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON, BluezGattCharacteristic1SkeletonClass))
#define BLUEZ_IS_GATT_CHARACTERISTIC1_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON))
#define BLUEZ_IS_GATT_CHARACTERISTIC1_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), BLUEZ_TYPE_GATT_CHARACTERISTIC1_SKELETON))

typedef struct _BluezGattCharacteristic1Skeleton BluezGattCharacteristic1Skeleton;
typedef struct _BluezGattCharacteristic1SkeletonClass BluezGattCharacteristic1SkeletonClass;
typedef struct _BluezGattCharacteristic1SkeletonPrivate BluezGattCharacteristic1SkeletonPrivate;

struct _BluezGattCharacteristic1Skeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  BluezGattCharacteristic1SkeletonPrivate *priv;
};

struct _BluezGattCharacteristic1SkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType bluez_gatt_characteristic1_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (BluezGattCharacteristic1Skeleton, g_object_unref)
#endif

BluezGattCharacteristic1 *bluez_gatt_characteristic1_skeleton_new (void);


G_END_DECLS

#endif /* __ORG_BLUEZ_GATTCHARACTERISTIC1_H__ */
