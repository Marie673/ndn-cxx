/**
 * @author: Jeff Thompson
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_DATA_HPP
#define NDN_DATA_HPP

#include "common.hpp"
#include "name.hpp"
#include "publisher-public-key-digest.hpp"
#include "key.hpp"
#include "c/data.h"

namespace ndn {

/**
 * A Signature holds the signature bits and other info representing the signature in a data packet.
 */
class Signature {
public:
  /**
   * Set the signatureStruct to point to the values in this signature object, without copying any memory.
   * WARNING: The resulting pointers in signatureStruct are invalid after a further use of this object which could reallocate memory.
   * @param signatureStruct a C ndn_Signature struct where the name components array is already allocated.
   */
  void get(struct ndn_Signature& signatureStruct) const;

  /**
   * Clear this signature, and set the values by copying from the ndn_Signature struct.
   * @param signatureStruct a C ndn_Signature struct
   */
  void set(const struct ndn_Signature& signatureStruct);

  const Blob& getDigestAlgorithm() const { return digestAlgorithm_; }

  const Blob& getWitness() const { return witness_; }

  const Blob& getSignature() const { return signature_; }
  
  const PublisherPublicKeyDigest& getPublisherPublicKeyDigest() const { return publisherPublicKeyDigest_; }
  PublisherPublicKeyDigest& getPublisherPublicKeyDigest() { return publisherPublicKeyDigest_; }
  
  const KeyLocator& getKeyLocator() const { return keyLocator_; }
  KeyLocator& getKeyLocator() { return keyLocator_; }

  void setDigestAlgorithm(const std::vector<unsigned char>& digestAlgorithm) { digestAlgorithm_ = digestAlgorithm; }
  void setDigestAlgorithm(const unsigned char *digestAlgorithm, unsigned int digestAlgorithmLength) 
  { 
    digestAlgorithm_ = Blob(digestAlgorithm, digestAlgorithmLength); 
  }

  void setWitness(const std::vector<unsigned char>& witness) { witness_ = witness; }
  void setWitness(const unsigned char *witness, unsigned int witnessLength) 
  { 
    witness_ = Blob(witness, witnessLength); 
  }

  void setSignature(const std::vector<unsigned char>& signature) { signature_ = signature; }
  void setSignature(const unsigned char *signature, unsigned int signatureLength) 
  { 
    signature_ = Blob(signature, signatureLength); 
  }

  void setPublisherPublicKeyDigest(const PublisherPublicKeyDigest& publisherPublicKeyDigest) { publisherPublicKeyDigest_ = publisherPublicKeyDigest; }
  
  void setKeyLocator(const KeyLocator& keyLocator) { keyLocator_ = keyLocator; }
  
  /**
   * Clear all the fields.
   */
  void clear()
  {
    digestAlgorithm_.reset();
    witness_.reset();
    signature_.reset();
    publisherPublicKeyDigest_.clear();
    keyLocator_.clear();
  }

private:
  Blob digestAlgorithm_; /**< if empty, the default is 2.16.840.1.101.3.4.2.1 (sha-256) */
  Blob witness_;
  Blob signature_;
  PublisherPublicKeyDigest publisherPublicKeyDigest_;
  KeyLocator keyLocator_;
};

/**
 * An MetaInfo holds the meta info which is signed inside the data packet.
 */
class MetaInfo {
public:
  MetaInfo() 
  {   
    type_ = ndn_ContentType_DATA;
    freshnessSeconds_ = -1;
  }

  /**
   * Set the metaInfoStruct to point to the values in this meta info object, without copying any memory.
   * WARNING: The resulting pointers in metaInfoStruct are invalid after a further use of this object which could reallocate memory.
   * @param metaInfoStruct a C ndn_MetaInfo struct where the name components array is already allocated.
   */
  void get(struct ndn_MetaInfo& metaInfoStruct) const;

  /**
   * Clear this meta info, and set the values by copying from the ndn_MetaInfo struct.
   * @param metaInfoStruct a C ndn_MetaInfo struct
   */
  void set(const struct ndn_MetaInfo& metaInfoStruct);

  double getTimestampMilliseconds() const { return timestampMilliseconds_; }
  
  ndn_ContentType getType() const { return type_; }
  
  int getFreshnessSeconds() const { return freshnessSeconds_; }
  
  const Name::Component& getFinalBlockID() const { return finalBlockID_; }
  
  void setTimestampMilliseconds(double timestampMilliseconds) { timestampMilliseconds_ = timestampMilliseconds; }
  
  void setType(ndn_ContentType type) { type_ = type; }
  
  void setFreshnessSeconds(int freshnessSeconds) { freshnessSeconds_ = freshnessSeconds; }
  
  void setFinalBlockID(const std::vector<unsigned char>& finalBlockID) { finalBlockID_ = Name::Component(finalBlockID); }
  void setFinalBlockID(const unsigned char *finalBlockID, unsigned int finalBlockIdLength) 
  { 
    finalBlockID_ = Name::Component(finalBlockID, finalBlockIdLength); 
  }
  
private:
  double timestampMilliseconds_; /**< milliseconds since 1/1/1970. -1 for none */
  ndn_ContentType type_;         /**< default is ndn_ContentType_DATA. -1 for none */
  int freshnessSeconds_;         /**< -1 for none */
  Name::Component finalBlockID_; /** size 0 for none */
};
  
class Data {
public:
  Data() 
  {
  }
  
  Data(const Name& name)
  : name_(name)
  {
  }
  
  Blob wireEncode(WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) const 
  {
    return wireFormat.encodeData(*this);
  }
  void wireDecode(const unsigned char *input, unsigned int inputLength, WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) 
  {
    wireFormat.decodeData(*this, input, inputLength);
  }
  void wireDecode(const std::vector<unsigned char>& input, WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) 
  {
    wireDecode(&input[0], input.size(), wireFormat);
  }
  
  /**
   * Set the dataStruct to point to the values in this interest, without copying any memory.
   * WARNING: The resulting pointers in dataStruct are invalid after a further use of this object which could reallocate memory.
   * @param dataStruct a C ndn_Data struct where the name components array is already allocated.
   */
  void get(struct ndn_Data& dataStruct) const;

  /**
   * Clear this data object, and set the values by copying from the ndn_Data struct.
   * @param dataStruct a C ndn_Data struct
   */
  void set(const struct ndn_Data& dataStruct);

  const Signature& getSignature() const { return signature_; }
  Signature& getSignature() { return signature_; }
  
  const Name& getName() const { return name_; }
  Name& getName() { return name_; }
  
  const MetaInfo& getMetaInfo() const { return metaInfo_; }
  MetaInfo& getMetaInfo() { return metaInfo_; }
  
  const Blob& getContent() const { return content_; }

  void setSignature(const Signature& signature) { signature_ = signature; }
  
  void setName(const Name& name) { name_ = name; }
  
  void setMetainfo(const MetaInfo& metaInfo) { metaInfo_ = metaInfo; }

  /**
   * Set the content to a copy of the data in the vector.
   * @param content A vector whose contents are copied.
   */
  void setContent(const std::vector<unsigned char>& content) { content_ = content; }
  void setContent(const unsigned char *content, unsigned int contentLength) 
  { 
    content_ = Blob(content, contentLength); 
  }
      
  /**
   * Set content to point to an existing byte array.  IMPORTANT: After calling this,
   * if you keep a pointer to the array then you must treat the array as immutable and promise not to change it.
   * @param content A pointer to a vector with the byte array.  This takes another reference and does not copy the bytes.
   */
  void setContent(const ptr_lib::shared_ptr<std::vector<unsigned char> > &content) { content_ = content; }
  void setContent(const ptr_lib::shared_ptr<const std::vector<unsigned char> > &content) { content_ = content; }

private:
  Signature signature_;
  Name name_;
  MetaInfo metaInfo_;
  Blob content_;
};
  
}

#endif
