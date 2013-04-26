/* Copyright (c) 2013 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "SocialInvocation.hpp"

#include <bb/system/InvokeRequest>

using namespace bb::system;

//! [0]
SocialInvocation::SocialInvocation(QObject *parent)
    : QObject(parent)
    , m_invokeManager(new InvokeManager(this))
{
}
//! [0]

QByteArray SocialInvocation::encodeQString(const QString& toEncode) const
{
    return toEncode.toUtf8();
}

//! [1]
void SocialInvocation::invoke(const QString &target, const QString &action,
                              const QString &mimetype, const QString &uri)
{
    // Create a new invocation request
    InvokeRequest request;

    request.setTarget(target);
    request.setAction(action);

    if (target == QLatin1String("com.rim.bb.app.facebook")) {
        QVariantMap payload;

        if (!uri.isEmpty()) {
            payload["object_type"] = mimetype;
            payload["object_id"] = uri;
        } else {
            // Open the BlackBerry North America page by default
            payload["object_type"] = "page";
            payload["object_id"] = "328506290597521";
        }

        request.setMetadata(payload);
    } else {
        request.setUri(uri);
    }

    m_invokeManager->invoke(request);
}
//! [1]
