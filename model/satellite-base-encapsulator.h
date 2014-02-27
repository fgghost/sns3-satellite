/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Jani Puttonen <jani.puttonen@magister.fi>
 */

#ifndef SAT_BASE_ENCAPSULATOR_H
#define SAT_BASE_ENCAPSULATOR_H

#include <ns3/packet.h>
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "satellite-queue.h"

#include "ns3/object.h"

namespace ns3 {


/**
 * \ingroup satellite
 * \brief This class implements a base encapsulator class. Basically
 * it is a packet container using SatQueue class. It does not support
 * encapsulator nor fragmentation / packing. Return Link Encapsulator (RLE)
 * and Generic Stream Encapsulator (GSE) are inherited from this base
 * class. Note, that this base class is meant to be utilized to enque
 * control packets.
 */
class SatBaseEncapsulator : public Object
{

public:

  /**
   * Default constructor not used
   */
  SatBaseEncapsulator ();

  /**
   * Constructor
   * \param source Configured source MAC addressd
   * \param dest Configured destination MAC address
   * \param flowId Flow identifier
   */
  SatBaseEncapsulator (Mac48Address source, Mac48Address dest, uint8_t flowId);

  virtual ~SatBaseEncapsulator ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * Callback to send packet to lower layer.
    * \param Ptr<Packet> the packet received
    */
  typedef Callback<void, Ptr<Packet> > ReceiveCallback;

  /**
   * Set the used queue from outside
   * \param queue Transmission queue
   */
  void SetQueue (Ptr<SatQueue> queue);

  /**
   * Get the queue instance
   * \param queue
   */
  Ptr<SatQueue> GetQueue ();

  /**
   * Method to set receive callback.
    * \param cb callback to invoke whenever a packet has been received and must
    *        be forwarded to the higher layers.
    */
  void SetReceiveCallback (ReceiveCallback cb);

  /**
   * Enqueue a packet to txBuffer.
   * \param p To be buffered packet
   */
  virtual void TransmitPdu (Ptr<Packet> p);

  /**
   * Notify a Tx opportunity to this base encapsulator. Note, that
   * this class does not do encapsulator nor do not support fragmentation.
   * Thus, enqueued packet must fit into the tx opportunity.
   */
  virtual Ptr<Packet> NotifyTxOpportunity (uint32_t bytes, uint32_t &bytesLeft);

  /**
   * Receive a packet. Note, that base encapsulator does not support
   * packet reception, since it assumes that packet receptions are
   * terminated already at lower layers.
   * \param p packet pointer received from lower layer
   */
  virtual void ReceivePdu (Ptr<Packet> p);

  /**
   * Get the buffered packets for this encapsulator
   * \return uint32_t buffered bytes
   */
  virtual uint32_t GetTxBufferSizeInBytes () const;

  /**
   * Get Head-of-Line packet buffering delay.
   * \return Time HoL buffering delay
   */
  virtual Time GetHolDelay () const;

  /**
   * Get minimum Tx opportunity in bytes, which takes the
   * assumed header sizes into account.
   * \return uint32_t minimum tx opportunity
   */
  virtual uint32_t GetMinTxOpportunityInBytes () const;

protected:

  /**
   * Source and destination mac addresses. Used to tag the Frame PDU
   * so that lower layers are capable of passing the packet to the
   * correct destination.
   */
  Mac48Address m_sourceAddress;
  Mac48Address m_destAddress;

  /**
   * Flow identifier
   */
  uint8_t m_flowId;

  /**
   * Used queue in satellite encapsulator
   */
  Ptr<SatQueue> m_txQueue;

  /**
   * Receive callback
   */
  ReceiveCallback m_rxCallback;

  /**
   * Used to inform of a PDU delivery
   */
  TracedCallback<uint16_t, uint8_t, uint32_t> m_txPdu;
  /**
   * Used to inform of a PDU reception
   */
  TracedCallback<uint16_t, uint8_t, uint32_t, uint64_t> m_rxPdu;

};


} // namespace ns3

#endif // SAT_BASE_ENCAPSULATOR_H