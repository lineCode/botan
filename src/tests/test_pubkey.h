/*
* (C) 2014,2015 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_TEST_PUBKEY_H__
#define BOTAN_TEST_PUBKEY_H__

#include "tests.h"

#include "test_rng.h"

#if defined(BOTAN_HAS_PUBLIC_KEY_CRYPTO)

#include <botan/pubkey.h>

namespace Botan_Tests {

class PK_Test : public Text_Based_Test
   {
   public:
      PK_Test(const std::string& algo,
              const std::string& test_src,
              const std::string& required_keys,
              const std::string& optional_keys = {})
         : Text_Based_Test(test_src, required_keys, optional_keys), m_algo(algo) {}

      std::string algo_name() const
         {
         return m_algo;
         }

   protected:
      std::vector<std::string> possible_providers(const std::string&) override;

   private:
      std::string m_algo;
   };

class PK_Signature_Generation_Test : public PK_Test
   {
   public:
      PK_Signature_Generation_Test(const std::string& algo,
                                   const std::string& test_src,
                                   const std::string& required_keys,
                                   const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      virtual std::string default_padding(const VarMap&) const
         {
         throw Test_Error("No default padding scheme set for " + algo_name());
         }

      virtual std::unique_ptr<Botan::Private_Key> load_private_key(const VarMap& vars) = 0;

      virtual Botan::RandomNumberGenerator* test_rng(const std::vector<uint8_t>& nonce) const
         {
         return new Fixed_Output_RNG(nonce);
         }

   private:
      Test::Result run_one_test(const std::string&, const VarMap& vars) override final;
   };

class PK_Signature_Verification_Test : public PK_Test
   {
   public:
      PK_Signature_Verification_Test(const std::string& algo,
                                     const std::string& test_src,
                                     const std::string& required_keys,
                                     const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      virtual std::string default_padding(const VarMap&) const
         {
         throw Test_Error("No default padding scheme set for " + algo_name());
         }

      virtual std::unique_ptr<Botan::Public_Key> load_public_key(const VarMap& vars) = 0;
   private:
      Test::Result run_one_test(const std::string& header, const VarMap& vars) override final;
   };

class PK_Signature_NonVerification_Test : public PK_Test
   {
   public:
      PK_Signature_NonVerification_Test(const std::string& algo,
                                        const std::string& test_src,
                                        const std::string& required_keys,
                                        const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      bool clear_between_callbacks() const override
         {
         return false;
         }

      virtual std::string default_padding(const VarMap&) const
         {
         throw Test_Error("No default padding scheme set for " + algo_name());
         }

      virtual std::unique_ptr<Botan::Public_Key> load_public_key(const VarMap& vars) = 0;
   private:
      Test::Result run_one_test(const std::string& header, const VarMap& vars) override final;
   };

class PK_Encryption_Decryption_Test : public PK_Test
   {
   public:
      PK_Encryption_Decryption_Test(const std::string& algo,
                                    const std::string& test_src,
                                    const std::string& required_keys,
                                    const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      virtual std::unique_ptr<Botan::Private_Key> load_private_key(const VarMap& vars) = 0;

      virtual std::string default_padding(const VarMap&) const
         {
         return "Raw";
         }
   private:
      Test::Result run_one_test(const std::string& header, const VarMap& vars) override final;
   };

class PK_Key_Agreement_Test : public PK_Test
   {
   public:
      PK_Key_Agreement_Test(const std::string& algo,
                            const std::string& test_src,
                            const std::string& required_keys,
                            const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      virtual std::unique_ptr<Botan::Private_Key> load_our_key(const std::string& header,
            const VarMap& vars) = 0;

      virtual std::vector<uint8_t> load_their_key(const std::string& header,
            const VarMap& vars) = 0;

      virtual std::string default_kdf(const VarMap&) const
         {
         return "Raw";
         }

   private:
      Test::Result run_one_test(const std::string& header, const VarMap& vars) override final;
   };

class PK_KEM_Test : public PK_Test
   {
   public:
      PK_KEM_Test(const std::string& algo,
                  const std::string& test_src,
                  const std::string& required_keys,
                  const std::string& optional_keys = "")
         : PK_Test(algo, test_src, required_keys, optional_keys) {}

      virtual std::unique_ptr<Botan::Private_Key> load_private_key(const VarMap& vars) = 0;
   private:
      Test::Result run_one_test(const std::string& header, const VarMap& vars) override final;
   };

class PK_Key_Generation_Test : public Test
   {
   protected:
      std::vector<Test::Result> run() override final;

      virtual std::vector<std::string> keygen_params() const = 0;

      virtual std::string algo_name() const = 0;

      std::vector<std::string> possible_providers(const std::string&) override;
   };

void check_invalid_signatures(Test::Result& result,
                              Botan::PK_Verifier& verifier,
                              const std::vector<uint8_t>& message,
                              const std::vector<uint8_t>& signature);

void check_invalid_ciphertexts(Test::Result& result,
                               Botan::PK_Decryptor& decryptor,
                               const std::vector<uint8_t>& plaintext,
                               const std::vector<uint8_t>& ciphertext);

}

#endif

#endif
