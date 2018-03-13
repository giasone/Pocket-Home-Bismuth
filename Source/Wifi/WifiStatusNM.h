
/**
 * @file WifiStatusNM.h
 * 
 * Connects to NetWorkManager to track and control wifi.
 */
#pragma once
#ifdef LINUX

#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include "WifiStatus.h"

class WifiStatusNM : public WifiStatus {
public:
    WifiStatusNM();
    virtual ~WifiStatusNM() override;

    /**
     * @return the list of all Wifi access points close enough to detect.
     */
    Array<WifiAccessPoint> nearbyAccessPoints() override;

    /**
     * @return the currently connected access point, or WifiAccessPoint::null()
     * if no access point is connected. 
     */
    WifiAccessPoint connectedAccessPoint() const override;

    /**
     * @return true iff wifi is currently turned on.
     */
    bool isEnabled() const override;

    /**
     * @return true iff the system is currently connected to a wifi access
     * point.
     */
    bool isConnected() const override;

    /**
     * Turns on the wifi radio.
     */
    void enableWifi() override;

    /**
     * Turns off the wifi radio.
     */
    void disableWifi() override;

    /**
     * Attempts to connect to a wifi access point.
     * @param ap defines the wifi access point to connect to.
     * @param psk wifi access point key, if ap is a secure connection.
     */
    void setConnectedAccessPoint
    (const WifiAccessPoint& ap, String psk = String()) override;

    /**
     * If connected to a wifi access point, this will close that connection.
     */
    void disconnect() override;

    /**
     * Inform all listeners when wifi is enabled or disabled
     */
    void handleWirelessEnabled();

    /**
     * Inform all listeners when a wifi connection is created or fails
     * to be created.
     */
    void handleWirelessConnected();

    /**
     * Save access point data after connecting to an access point.
     */
    void handleConnectedAccessPoint();
private:

    /**
     * Attempt to open a connection to the network manager.
     * @return true iff the connection was made and nmClient was initialized.
     */
    bool connectToNetworkManager();

    /**
     * Wifi thread loop:
     * While the wifi status thread is running, regularly check the network
     * manager for wifi network changes.
     */
    void run() override;

    /**
     * Generate a unique hash string identifying a wifi access point.
     * Adapted from network-manager-applet src/utils/utils.c 
     * @param ap
     */
    char* hashAP(NMAccessPoint* ap);

    /**
     * @param ap
     * @return true iff ap is a secure access point.
     */
    bool resolveAPSecurity(NMAccessPoint* ap);

    /**
     * @param ap
     * @return WifiAccessPoint data read from a NMAccessPoint
     */
    WifiAccessPoint createNMWifiAccessPoint(NMAccessPoint* ap);

    /**
     * @param wdev
     * @return WifiAccessPoint data of the access point connected to wdev, or
     * WifiAccessPoint::null() if no connection is found.
     */
    WifiAccessPoint getNMConnectedAP(NMDeviceWifi *wdev);
    
    /**
     * Close a specific connection on a specific wireless device.
     * @param nmdevice
     * @param conn
     */
    void removeNMConnection(NMDevice *nmdevice, NMActiveConnection *conn);
    
    /**
     * @param key
     * @return true iff key has the correct format for a WEP key.
     */
    bool isValidWEPKeyFormat(String key);
    
    /**
     * @param phrase
     * @return true iff phrase has the correct format for a WEP passphrase.
     */
    bool isValidWEPPassphraseFormat(String phrase);
  
    Array<WifiStatus::Listener*> listeners;
    WifiAccessPoint connectedAP;
    bool enabled = false;
    bool connected = false;
    bool connecting = false;
    NMClient *nmClient = nullptr;
    NMDevice* nmDevice = nullptr;
    GMainLoop *loop;
    GMainContext *context;
};
#endif // LINUX
