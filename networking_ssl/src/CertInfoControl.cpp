/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#include "CertInfoControl.hpp"

CertInfoControl::CertInfoControl () :   QObject()
{
    m_dialogVisible = false;
}

void CertInfoControl::onCertificateChainChanged (const QList < QSslCertificate > &chain )
{
    m_chain = chain;

    // Clear the model
    m_model.clear();

    // Add a new entry for each certificate from
    // the chain to the model
    for (int i = 0; i < m_chain.size(); ++i)
    {
        const QSslCertificate &cert = m_chain.at(i);

        const QString chainInfo =
                QString("%1%2 (%3)")
                .arg(i == 0 ? QString() : QString("Issued by: "))
                .arg(cert.subjectInfo(QSslCertificate::Organization))
                .arg(cert.subjectInfo(QSslCertificate::CommonName));

        m_model.append(chainInfo);
    }

    // Select the first certificate in the list
    setCurrentCertificate(QVariantList() << QVariant(0));
}

bb::cascades::DataModel* CertInfoControl::model () const
{
    return const_cast < bb::cascades::QListDataModel < QString >* >(&m_model);
}

QString CertInfoControl::certificateInfo () const
{
    return m_certificateInfo;
}

void CertInfoControl::setVisible ( bool visible )
{
    if (m_dialogVisible == visible) return;

    m_dialogVisible = visible;
    emit visibleChanged();
}

bool CertInfoControl::visible () const
{
    return m_dialogVisible;
}

void CertInfoControl::onViewCertificateChain ()
{
    setVisible(true);
}

void CertInfoControl::setCurrentCertificate ( const QVariantList &indexPath )
{
    // Get the index of the certificate that the user has selected
    const int index = indexPath.at(0).toInt();

    if (index >= 0 && index < m_chain.size())
    {
        const QSslCertificate &cert = m_chain.at(index);

        // Add the selected certificate data and update the property
        QStringList certDataItems;
        certDataItems
            << "=== Certificate Information ==="
            << "Organization: "     + cert.subjectInfo(QSslCertificate::Organization)
            << "Subunit: "          + cert.subjectInfo(QSslCertificate::OrganizationalUnitName)
            << "Country: "          + cert.subjectInfo(QSslCertificate::CountryName)
            << "Locality: "         + cert.subjectInfo(QSslCertificate::LocalityName)
            << "State/Province: "   + cert.subjectInfo(QSslCertificate::StateOrProvinceName)
            << "Common Name: "      + cert.subjectInfo(QSslCertificate::CommonName)
            << "\n=== Certificate Issuer Information ==="
            << "Organization: "     + cert.issuerInfo(QSslCertificate::Organization)
            << "Unit Name: "        + cert.issuerInfo(QSslCertificate::OrganizationalUnitName)
            << "Country: "          + cert.issuerInfo(QSslCertificate::CountryName)
            << "Locality: "         + cert.issuerInfo(QSslCertificate::LocalityName)
            << "State/Province: "   + cert.issuerInfo(QSslCertificate::StateOrProvinceName)
            << "Common Name: "      + cert.issuerInfo(QSslCertificate::CommonName);

        m_certificateInfo = certDataItems.join("\n");
    }
    else
    {
        m_certificateInfo.clear();
    }

    emit certificateInfoChanged();
}
